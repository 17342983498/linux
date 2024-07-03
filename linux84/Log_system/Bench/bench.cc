#include "../Log/logsystem.h"
#include <vector>
#include <thread>
#include <chrono>

void bench(const std::string &logger_name, size_t thread_count, size_t msg_count, size_t msg_len)
{
    std::cout << "\t测试日志：" << msg_count << " 条，总大小：" << msg_count * msg_len / 1024 << " KB" << std::endl;
    // 1. 获取日志器
    LogSystem::Logger::ptr logger = LogSystem::GetLogger(logger_name);
    if (logger.get() == nullptr)
    {
        return;
    }
    // 2. 组织指定长度的日志消息
    std::string msg(msg_len - 1, 'a'); // 少一个字节是为了给末尾添加换行
    // 3. 创建指定数量的线程
    std::vector<std::thread> threads;
    std::vector<double> cost_time_array(thread_count); // 存储每个线程所花费的花间
    size_t thread_msg_count = msg_count / thread_count;
    for (size_t i = 0; i < thread_count; i++)
    {
        threads.emplace_back([&, i]()
                             {
                // 4. 线程函数内部开始计时
                auto start = std::chrono::high_resolution_clock::now();
                // 5. 开始循环写日志
                for (size_t j = 0; j < thread_msg_count;j++)
                {
                    logger->fatal("%s", msg.c_str());
                }
                // 6. 线程函数内部结束计时
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> cost_time = end - start;
                cost_time_array[i] = cost_time.count();
                std::cout << "\t线程" << i << "：" << "\t输出日志数量：" << thread_msg_count << ", 耗时：" << cost_time_array[i] << "s" << std::endl; });
    }
    for (size_t i = 0; i < thread_count; i++)
    {
        threads[i].join();
    }
    // 7. 计算总耗时：在多线程的场景下，因为线程是并发处理的，所以总耗时并非所有线程花费的时间加和，而是耗时最多的线程的耗时时间为总耗时
    double max_time = cost_time_array[0];
    for (size_t i = 1; i < thread_count; i++)
    {
        max_time > cost_time_array[i] ? max_time : max_time = cost_time_array[i];
    }
    size_t msgcount_per_sec = msg_count / max_time;                     // 每秒输出的日志数量
    size_t msgsize_per_sec = (msg_count * msg_len) / (max_time * 1024); // 每秒输出的日志大小
    // size_t size_per_sec = (msgcount_per_sec * msg_len) / 1024;
    //  8. 进行输出打印
    std::cout << "\t每秒输出的日志数量：" << msgcount_per_sec << " 条" << std::endl;
    std::cout << "\t每秒输出的日志大小：" << msgsize_per_sec << " KB" << std::endl;
}

void sync_bench()
{
    std::unique_ptr<LogSystem::LoggerBuild> builder(new LogSystem::GlobalLoggerBuilder());
    builder->buildLoggerName("sync_logger");
    builder->buildLoggerLevel(LogSystem::LogLevel::value::DEBUG);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildLoggerType(LogSystem::LoggerType::LOGGER_SYNC);
    builder->buildSink<LogSystem::FileSink>("./logfile/sync.log");
    builder->build();
    bench("sync_logger", 5, 5000000, 100);
}

void async_bench()
{
    std::unique_ptr<LogSystem::LoggerBuild> builder(new LogSystem::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildEnableUnsafeAsync();
    builder->buildLoggerLevel(LogSystem::LogLevel::value::DEBUG);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildLoggerType(LogSystem::LoggerType::LOGGER_ASYNC);
    builder->buildSink<LogSystem::FileSink>("./logfile/async.log");
    builder->build();
    bench("async_logger", 1, 5000000, 100);
}

int main()
{
    async_bench();
    return 0;
}