
#ifndef __M_BUFFER_H__
#define __M_BUFFER_H__

#include <iostream>
#include <vector>
#include <cassert>

namespace LogSystem
{
#define DEFAULT_BUFFER_SIZE (100 * 1024 * 1024)
    class Buffer
    {
    public:
        Buffer() : _buffer(DEFAULT_BUFFER_SIZE), _reader_idx(0), _writer_idx(0) {}

        // 向缓冲区内写数据
        void Push(const char *data, size_t len)
        {
            // 先判断该缓冲区是否还有足够的空间写入数据
            IncreaseSpace(len);
            std::copy(data, data + len, &_buffer[_writer_idx]);
            MoveWrite(len);
        }

        // 可读的起始位置
        const char *ReadBegin()
        {
            return &_buffer[_reader_idx];
        }

        // 可写空间大小
        size_t WriteAbleSize()
        {
            return (_buffer.size() - _writer_idx);
        }

        // 可读数据的长度
        size_t ReadAbleLength()
        {
            return (_writer_idx - _reader_idx);
        }

        // 读指针偏移
        void MoveRead(size_t len)
        {
            assert(len <= ReadAbleLength());
            _reader_idx += len;
        }

        // 初始化缓冲区，重置读写位置
        void Reset()
        {
            _reader_idx = _writer_idx = 0;
        }

        void Swap(Buffer &buffer)
        {
            _buffer.swap(buffer._buffer);
            std::swap(_reader_idx, buffer._reader_idx);
            std::swap(_writer_idx, buffer._writer_idx);
        }

        bool empty()
        {
            return _reader_idx == _writer_idx;
        }

    private:
        // 写指针向后偏移
        void MoveWrite(size_t len)
        {
            assert((_writer_idx + len) <= _buffer.size());
            _writer_idx += len;
        }

        void IncreaseSpace(int len)
        {
            while (len > WriteAbleSize())
            {
                _buffer.resize(_buffer.capacity() * 2);
            }
        }

    private:
        std::vector<char> _buffer;
        size_t _reader_idx; // 本质就是一个下标
        size_t _writer_idx;
    };
}

#endif