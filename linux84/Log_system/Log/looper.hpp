/*异步日志器*/

#ifndef __M_LOOPER_H__
#define __M_LOOPER_H__

#include <mutex>
#include <thread>
#include <condition_variable>
#include "buffer.hpp"
#include <functional>
#include <memory>
#include <atomic>

namespace LogSystem
{
    using Functor = std::function<void(Buffer &)>;
    enum class AsyncType
    {
        ASYNC_SAFE,
        ASYNC_UNSAFE
    };

    class AsyncLooper
    {
    public:
        using ptr = std::shared_ptr<AsyncLooper>;
        AsyncLooper(const Functor &cb, AsyncType looper_type = AsyncType::ASYNC_SAFE)
            : _stop_flag(false), _thread(std::thread(&AsyncLooper::threadEntry, this)), _callback(cb), _looper_type(looper_type)
        {
        }

        ~AsyncLooper()
        {
            stop();
        }

        void stop()
        {
            _stop_flag = true;
            _con_cond.notify_all(); // 在工作器停止之后，唤醒所有的工作线程去处理消费缓冲区中的数据
            _thread.join();
        }

        void push(const char *data, size_t len)
        {
            // 先加锁
            std::unique_lock<std::mutex> lock(_mutex);
            // 条件等待，若数据缓冲区有足够的空间无需等待，若空间不足就等待
            if (_looper_type == AsyncType::ASYNC_SAFE)
            {
                // 该逻辑就是判断异步工作器面对空间不足选择安全还是非安全的方式
                // 只有是安全的方式才会阻塞等待，等待缓冲区交换之后被唤醒
                // 如果是非安全的则直接扩容，扩容就在下面的Push函数当中
                _pro_cond.wait(lock, [&]()
                               { return _pro_buffer.WriteAbleSize() >= len; });
            }
            // 压入数据
            _pro_buffer.Push(data, len);
            // 唤醒工作线程
            _con_cond.notify_one();
        }

    private:
        // 线程入口函数--对消费缓冲区中的数据进行处理，处理完毕后，初始化缓冲区再交换缓冲区
        void threadEntry()
        {
            while (1)
            // while(!_stop_flag)
            {
                {
                    // 先加锁
                    std::unique_lock<std::mutex> lock(_mutex);
                    if (_stop_flag && _pro_buffer.empty())
                        break;
                    // 条件等待，有两种情况：日志器退出、生产buffer有数据就会被唤醒，否则继续阻塞等待。
                    _con_cond.wait(lock, [&]()
                                   { return _stop_flag || !_pro_buffer.empty(); });
                    // 交换生产缓冲区和消费缓冲区
                    _con_buffer.Swap(_pro_buffer);
                    // 唤醒业务线程，同样在这里也对安全或非安全进行了判断，只有安全的模式才需要唤醒阻塞的业务线程
                    if (_looper_type == AsyncType::ASYNC_SAFE)
                        _pro_cond.notify_all();
                }
                _callback(_con_buffer);
                _con_buffer.Reset();
            }
        }
        Functor _callback; // 对缓冲区数据进行处理的回调函数
    private:
        Buffer _pro_buffer;
        Buffer _con_buffer;
        std::mutex _mutex;
        std::condition_variable _pro_cond; // 条件变量
        std::condition_variable _con_cond;
        std::atomic<bool> _stop_flag; // 工作器停止标志
        std::thread _thread;          // 异步工作器对应的工作线程
        AsyncType _looper_type;
    };
}

#endif