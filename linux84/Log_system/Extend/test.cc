// #include "util.hpp"
// #include "level.hpp"
// #include "message.hpp"
// #include "format.hpp"
// #include "sink.hpp"
// #include "logger.hpp"
// #include "buffer.hpp"

#include "../Log/logsystem.h"

enum class TimeGap
{
    GAP_SECOND,
    GAP_MINUTE,
    GAP_HOUR,
    GAP_DAY,
};

class RollByTimeSink : public LogSystem::LogSink
{
public:
    RollByTimeSink(const std::string& basename, TimeGap gap_type) : _basename(basename)
    {
        switch (gap_type)
        {
            case TimeGap::GAP_SECOND: _gap_size = 2;
            case TimeGap::GAP_MINUTE: _gap_size = 60;
            case TimeGap::GAP_HOUR: _gap_size = 3600;   
            case TimeGap::GAP_DAY: _gap_size = 3600 * 24;
        }
        _cur_gap = _gap_size == 1?LogSystem::util::Date::getTime():LogSystem::util::Date::getTime() % _gap_size;
        //_cur_gap = LogSystem::util::Date::getTime() % _gap_size;
        //old = LogSystem::util::Date::getTime();
        std::string filename = createNewFile();
        //1. 创建日志文件所在的目录
        LogSystem::util::File::createDirectory(LogSystem::util::File::path(filename));
        //2. 创建并打开日志文件
        _ofs.open(filename, std::ios::binary | std::ios::app);
        assert(_ofs.is_open());
    }

    void log(const char* data, size_t len)
    {
        time_t cur = LogSystem::util::Date::getTime();
        if ((cur % _gap_size) != _cur_gap)
        //if(((cur % _gap_size) - _cur_gap) > _gap_size)
        //if(((cur % _gap_size) - (old % _gap_size)) > _gap_size)
        {
            //old = cur;
            _ofs.close();
            std::string filename = createNewFile();
            _ofs.open(filename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        _ofs.write(data, len);
        assert(_ofs.good());
    }
private:
    std::string createNewFile()
    {
        time_t t = LogSystem::util::Date::getTime();
        struct tm lt;
        localtime_r(&t, &lt);
        std::stringstream filename;
        filename << _basename;
        filename << lt.tm_year + 1900;
        filename << lt.tm_mon + 1;
        filename << lt.tm_mday;
        filename << lt.tm_hour;
        filename << lt.tm_min;
        filename << lt.tm_sec;
        filename << ".log";
        return filename.str();
    }
private:
    std::string _basename;
    std::ofstream _ofs;
    size_t _gap_size;
    size_t _cur_gap;
    time_t old;
};

// void test_log()
// {
//     LogSystem::Logger::ptr logger = LogSystem::LoggerManager::getInstance().GetLogger("async_logger");

//     DEBUG("%s", "测试日志");
//     WARN("%s", "测试日志");
//     ERROR("%s", "测试日志");
//     INFO("%s", "测试日志");
//     FATAL("%s", "测试日志");

//     // size_t count = 0;
//     // while (count < 500000)
//     // {
//     //     FATAL("测试日志-%d", count++);
//     // }
    
//     /*logger->debug("%s", "测试日志");
//     logger->warn("%s", "测试日志");
//     logger->error("%s", "测试日志");
//     logger->info("%s", "测试日志");
//     logger->fatal("%s", "测试日志");

//     size_t count = 0;
//     while (count < 500000)
//     {
//         logger->fatal("测试日志-%d", count++);
//     } */

// }


/* 在没有定义日志器的全局接口之前，需要用户自己来操作单例对象
void test_log()
{
    LogSystem::Logger::ptr logger = LogSystem::LoggerManager::getInstance().GetLogger("async_logger");
    logger->debug(__FILE__, __LINE__, "%s", "测试日志");
    logger->warn(__FILE__, __LINE__, "%s", "测试日志"); 
    logger->error(__FILE__, __LINE__, "%s", "测试日志");
    logger->info(__FILE__, __LINE__, "%s", "测试日志");
    logger->fatal(__FILE__, __LINE__, "%s", "测试日志");

    size_t count = 0;
    while (count < 500000)
    {
        logger->fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    } 
}
*/


int main()
{
    // //测试引入全局日志器管理的方式
    // std::unique_ptr<LogSystem::LoggerBuild> builder(new LogSystem::GlobalLoggerBuilder());
    // builder->buildLoggerName("async_logger");
    // builder->buildLoggerLevel(LogSystem::LogLevel::value::WARN);
    // builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    // builder->buildLoggerType(LogSystem::LoggerType::LOGGER_ASYNC);
    // builder->buildSink<RollByTimeSink>("./logfile/async.log", TimeGap::GAP_SECOND);
    // LogSystem::Logger::ptr logger = builder->build();//负责创建一个日志器对象
    // //test_log();

    // size_t cur = LogSystem::util::Date::getTime();
    // while (LogSystem::util::Date::getTime() < cur + 5)
    // {
    //     logger->fatal("测试日志");
    //     usleep(1000);
    // }

    // size_t count = 0;
    // while (count < 50000)
    // {
    //     logger->fatal("测试日志-%d", count++);
    // }

    

    /*先从一个文件中读取数据放到buffer中，再将buffer中的数据写入到新文件中
    std::ifstream ifs("./logfile/test.log", std::ios::binary);
    if (ifs.is_open() == false) { return -1; }
    ifs.seekg(0, std::ios::end);//读写位置跳转到文件末尾
    size_t fsize = ifs.tellg();//获取当前位置较于起始位置的偏移量
    ifs.seekg(0, std::ios::beg);//再回到文件起始位置
    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);
    if (ifs.good() == false)
    {
        std::cout << "read error!" << std::endl;
    }
    ifs.close();

    LogSystem::Buffer buffer;
    for (int i = 0; i < body.size(); i++)
    {
        buffer.Push(&body[i], 1);
    }
    std::ofstream ofs("./logfile/tmp.log", std::ios::binary);
    size_t size = buffer.ReadAbleLength();
    for (int i = 0; i < size; i++)
    {
        ofs.write(buffer.ReadBegin(), 1);
        buffer.MoveRead(1);
    }
    ofs.close();*/

    
    /*该段代码是引入了日志器后
    std::string logger_name = "sync_logger";
    LogSystem::LogLevel::value limit = LogSystem::LogLevel::value::WARN;
    LogSystem::Formatter::ptr fmt(new LogSystem::Formatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n"));
    LogSystem::LogSink::ptr stdout_lsp = LogSystem::SinkFactory::create<LogSystem::StdoutSink>();
    LogSystem::LogSink::ptr file_lsp = LogSystem::SinkFactory::create<LogSystem::FileSink>("./logfile/test.log");
    LogSystem::LogSink::ptr roll_lsp = LogSystem::SinkFactory::create<LogSystem::RollSizeSink>("./logfile/roll-", 1024 * 1024);
    std::vector<LogSystem::LogSink::ptr> sinks = { stdout_lsp, file_lsp, roll_lsp };
    LogSystem::Logger::ptr logger(new LogSystem::Synclogger(logger_name, limit, fmt, sinks));

    logger->debug(__FILE__, __LINE__, "%s", "测试日志");
    logger->warn(__FILE__, __LINE__, "%s", "测试日志"); 
    logger->error(__FILE__, __LINE__, "%s", "测试日志");
    logger->info(__FILE__, __LINE__, "%s", "测试日志");
    logger->fatal(__FILE__, __LINE__, "%s", "测试日志");

    size_t count = 0;
    while (count < 500000)
    {
        logger->fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    }*/


    /*该段代码是对日志器进行了管理，选用的方式是局部日志器管理方式
    std::unique_ptr<LogSystem::LoggerBuild> builder(new LogSystem::LocalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildEnableUnsafeAsync();
    builder->buildLoggerLevel(LogSystem::LogLevel::value::WARN);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildLoggerType(LogSystem::LoggerType::LOGGER_ASYNC);
    builder->buildSink<LogSystem::StdoutSink>();
    builder->buildSink<LogSystem::FileSink>("./logfile/async.log");
    LogSystem::Logger::ptr logger = builder->build();


    logger->debug(__FILE__, __LINE__, "%s", "测试日志");
    logger->warn(__FILE__, __LINE__, "%s", "测试日志"); 
    logger->error(__FILE__, __LINE__, "%s", "测试日志");
    logger->info(__FILE__, __LINE__, "%s", "测试日志");
    logger->fatal(__FILE__, __LINE__, "%s", "测试日志");

    size_t count = 0;
    while (count < 500000)
    {
        logger->fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    }*/



    LogSystem::LogMessage msg(LogSystem::LogLevel::value::INFO, 128, "main.c", "root", "格式化功能能测试...");
    LogSystem::Formatter fmt;
    std::string str = fmt.format(msg);
    std::cout << str << std::endl;

    LogSystem::FileSink::ptr stdout_lsp = LogSystem::SinkFactory::create<LogSystem::StdoutSink>();
    LogSystem::FileSink::ptr file_lsp = LogSystem::SinkFactory::create<LogSystem::FileSink>("./logfile/test.log");
    LogSystem::FileSink::ptr roll_lsp = LogSystem::SinkFactory::create<LogSystem::RollSizeSink>("./logfile/roll-", 1024 * 1024);

    LogSystem::FileSink::ptr time_lsp = LogSystem::SinkFactory::create<RollByTimeSink>("./logfile/roll-", TimeGap::GAP_SECOND);
    time_t t = LogSystem::util::Date::getTime();
    while (t > LogSystem::util::Date::getTime() - 5)
    {
        time_lsp->log(str.c_str(), str.size());
    }

    stdout_lsp->log(str.c_str(), str.size());
    file_lsp->log(str.c_str(), str.size());
    size_t cursize = 0;
    size_t count = 0;
    while (cursize < 1024 * 1024 * 10)
    {
        std::string tmp = std::to_string(count++) + str;
        roll_lsp->log(tmp.c_str(), tmp.size());
        cursize += tmp.size();
    }
    

    return 0; 
}