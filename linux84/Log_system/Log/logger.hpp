/*完成日志器模块
    1. 抽象日志器基类
    2. 派生出不同的子类（同步日志器类 & 异步日志器类）
*/

#ifndef __M_LOG_H__
#define __M_LOG_H__

#define _GUN_SOURCE
#include "util.hpp"
#include "level.hpp"
#include "format.hpp"
#include "sink.hpp"
#include "looper.hpp"
#include <atomic>
#include <mutex>
#include <cstdarg>
#include <unordered_map>

namespace LogSystem
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;
        Logger(const std::string &logger_name, LogLevel::value level, Formatter::ptr& formatter, std::vector<LogSink::ptr>& sinks)
            :_logger_name(logger_name), _limit_level(level), _formatter(formatter), _sinks(sinks.begin(),sinks.end())
        {}

        const std::string& GetLoggerName()
        {
            return _logger_name;
        }
        
        //完成构造日志消息对象过程并进行格式化，得到格式化后的日志消息字符串--然后进行落地输出
        void debug(const std::string &file, size_t line, const std::string& fmt, ...)
        {
            //通过传入的参数构造出一个日志消息对象，进行日志的格式化，落地
            //1. 判断当前日志是否到达了输出等级
            if (LogLevel::value::DEBUG < _limit_level) { return; }
            //2. 对fmt格式化字符串和不定参进行字符串组织--得到完整日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap); 
            serialize(LogLevel::value::DEBUG, file, line, res);
            free(res);
        }
        void info(const std::string &file, size_t line, const std::string& fmt, ...)
        {
            //通过传入的参数构造出一个日志消息对象，进行日志的格式化，落地
            //1. 判断当前日志是否到达了输出等级
            if (LogLevel::value::INFO < _limit_level) { return; }
            //2. 对fmt格式化字符串和不定参进行字符串组织--得到完整日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap);
            serialize(LogLevel::value::INFO, file, line, res);
            free(res);
        }
        
        void warn(const std::string &file, size_t line, const std::string& fmt, ...)
        {
            //通过传入的参数构造出一个日志消息对象，进行日志的格式化，落地
            //1. 判断当前日志是否到达了输出等级
            if (LogLevel::value::WARN < _limit_level) { return; }
            //2. 对fmt格式化字符串和不定参进行字符串组织--得到完整日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap);
            serialize(LogLevel::value::WARN, file, line, res);
            free(res);
        }
        void error(const std::string &file, size_t line, const std::string& fmt, ...)
        {
            //通过传入的参数构造出一个日志消息对象，进行日志的格式化，落地
            //1. 判断当前日志是否到达了输出等级
            if (LogLevel::value::ERROR < _limit_level) { return; }
            //2. 对fmt格式化字符串和不定参进行字符串组织--得到完整日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap);
            serialize(LogLevel::value::ERROR, file, line, res);
            free(res);
        }
        void fatal(const std::string &file, size_t line, const std::string& fmt, ...)
        {
            //通过传入的参数构造出一个日志消息对象，进行日志的格式化，落地
            //1. 判断当前日志是否到达了输出等级
            if (LogLevel::value::FATAL < _limit_level) { return; }
            //2. 对fmt格式化字符串和不定参进行字符串组织--得到完整日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap);
            serialize(LogLevel::value::FATAL, file, line, res);
            free(res);
        }
    protected:
        void serialize(LogLevel::value level, const std::string& file, size_t line, char* str)
        {
            //3. 构造LogMsg对象
            LogMessage msg(level, line, file, _logger_name, str);
            //4. 通过格式化工具对LogMsg进行格式化，
            std::stringstream ss;
            _formatter->format(ss, msg);
            //5. 日志落地
            log(ss.str().c_str(), ss.str().size());
        }
        //抽象接口完成实际的落地输出--不同的日志器会有不同的实际落地方式
        virtual void log(const char* data, size_t len) = 0;
    protected:
        std::mutex _mutex;
        std::string _logger_name;
        std::atomic<LogLevel::value> _limit_level;
        Formatter::ptr _formatter;
        std::vector<LogSink::ptr> _sinks;
    };

    class Synclogger :public Logger
    {
    public:
        Synclogger(const std::string &logger_name, LogLevel::value level, Formatter::ptr& formatter, std::vector<LogSink::ptr>& sinks)
            :Logger(logger_name, level, formatter, sinks)
        {}
    protected:
        //同步日志器是将日志直接通过落地模块句柄进行日志落地
        void log(const char* data, size_t len)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_sinks.empty()) return;
            for (auto& sink : _sinks)
            {
                sink->log(data, len);
            }
        }
    };

    class Asynclogger :public Logger
    {
    public:
        Asynclogger(const std::string& logger_name,
            LogLevel::value level,
            Formatter::ptr& formatter,
            std::vector<LogSink::ptr>& sinks,
            AsyncType looper_type)
            :Logger(logger_name, level, formatter, sinks),
            _looper(std::make_shared<AsyncLooper>(std::bind(&Asynclogger::realLog, this, std::placeholders::_1), looper_type))
        {}

        void log(const char* data, size_t len)//将数据写入缓冲区
        {
            _looper->push(data, len);
        }

        //设计一个实际落地函数，就是将缓冲区中的数据落地
        void realLog(Buffer& buf)
        {
            if (_sinks.empty()) return;
            for (auto& sink : _sinks)
            {
                sink->log(buf.ReadBegin(), buf.ReadAbleLength());
            }
        }
    private:
        AsyncLooper::ptr _looper;
    };

    enum class LoggerType
    {
        LOGGER_SYNC,
        LOGGER_ASYNC
    };

//上面的在建造日志器的时候用户需要给出关于日志器的一些选项比较麻烦，而下面的方式可以直接通过调用函数的方式来完成
    
    class LoggerBuild
    {
    public:
        LoggerBuild() :_logger_type(LoggerType::LOGGER_SYNC), _limit_level(LogLevel::value::DEBUG), _looper_type(AsyncType::ASYNC_SAFE) {}
        void buildLoggerType(LoggerType logger_type) { _logger_type = logger_type; }
        void buildEnableUnsafeAsync() { _looper_type = AsyncType::ASYNC_UNSAFE; }
        void buildLoggerName(const std::string& logger_name) { _logger_name = logger_name; }
        void buildLoggerLevel(LogLevel::value limit_level) { _limit_level = limit_level; }
        void buildFormatter(const std::string& pattern)
        {
            _formatter = std::make_shared<Formatter>(pattern); 
        }
        template<typename SinkType, typename ...Args>
        void buildSink(Args &&...args)
        {
            LogSink::ptr psink = SinkFactory::create<SinkType>(std::forward<Args>(args)...);//完美转发
            _sinks.push_back(psink);
        }
        virtual Logger::ptr build() = 0;
    protected:
        LoggerType _logger_type;
        AsyncType _looper_type;
        std::string _logger_name;
        LogLevel::value _limit_level;
        Formatter::ptr _formatter;
        std::vector<LogSink::ptr> _sinks;
    };

    //派生出了具体的建造者类：局部日志器建造者 and 全局日志器建造者
    class LocalLoggerBuilder : public LoggerBuild
    {
    public:
        Logger::ptr build() override
        {
            assert(_logger_name.empty() == false);
            if (_formatter.get() == nullptr)
            {
                _formatter = std::make_shared<Formatter>();
            }
            if (_sinks.empty())
            {
                buildSink<StdoutSink>();
            }
            if (_logger_type == LoggerType::LOGGER_ASYNC)
            {
                return std::make_shared<Asynclogger>(_logger_name, _limit_level, _formatter, _sinks, _looper_type);
            }
            else
                return std::make_shared<Synclogger>(_logger_name, _limit_level, _formatter, _sinks);//在这里是创建了同步日志器的一个对象
                //给出了将来要落地的一些参数要求
        }
    };

    //日志器管理器，通过设计单例模式可以在程序的任一位置获取相同的单例对象，获取其中的日志器进行日志输出
    class LoggerManager
    {
    public:
        static LoggerManager& getInstance()
        {
            //在C++11之后，针对局部静态变量，编译器在编译层面实现了线程安全
            //当局部静态变量在没有构造好之前，其他线程进入进入后就会阻塞
            static LoggerManager eton;
            return eton;
        }
        void AddLogger(Logger::ptr& logger)
        {
            if (!HasLogger(logger->GetLoggerName()))
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _loggers.insert(std::make_pair(logger->GetLoggerName(), logger));
            }
        }

        bool HasLogger(const std::string& name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _loggers.find(name);
            if (it == _loggers.end())
                return false;
            return true;
        }

        Logger::ptr GetLogger(const std::string& name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _loggers.find(name);
            if (it == _loggers.end())
                return Logger::ptr();
            return it->second;
        }
        Logger::ptr RootLogger()
        {
            return _root_logger;
        }

        LoggerManager()
        {
            std::unique_ptr<LogSystem::LoggerBuild> builder(new LogSystem::LocalLoggerBuilder());
            builder->buildLoggerName("root");
            //builder->buildEnableUnsafeAsync();//默认是安全地
            builder->buildLoggerLevel(LogSystem::LogLevel::value::DEBUG);
            //builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");//也有默认的
            //builder->buildLoggerType(LogSystem::LoggerType::LOGGER_ASYNC);//默认
            //builder->buildSink<LogSystem::StdoutSink>();//默认
            _root_logger = builder->build();
            _loggers.insert(std::make_pair("root", _root_logger));
        }
    private:
        std::mutex _mutex;
        Logger::ptr _root_logger;//默认日志器
        std::unordered_map<std::string, Logger::ptr> _loggers;
    };


    //在上面有了一个日志器的管理器，那么用户就可以通过单例对象将自己设计的某个日志器添加其中并进行全局管理
    //但是这样的操作无疑增加了用户的操作复杂度，因为要通过该管理器的单例对象调用添加函数
    //为了简化用户的使用度，设计一个全局日志器的建造者，在局部的基础之上加了一个功能：将日志器的单例对象添加其中
    class GlobalLoggerBuilder : public LoggerBuild
    {
    public:
        Logger::ptr build() override
        {
            assert(_logger_name.empty() == false);
            if (_formatter.get() == nullptr)
            {
                _formatter = std::make_shared<Formatter>();
            }
            if (_sinks.empty())
            {
                buildSink<StdoutSink>();
            }
            Logger::ptr logger;//将来所创建出来的logger对象，后面将其添加到日志器管理器中（通过单例对象）
            if (_logger_type == LoggerType::LOGGER_ASYNC)
            {
                logger = std::make_shared<Asynclogger>(_logger_name, _limit_level, _formatter, _sinks, _looper_type);
            }
            else
            {
                logger = std::make_shared<Synclogger>(_logger_name, _limit_level, _formatter, _sinks);//在这里是创建了同步日志器的一个对象
                //给出了将来要落地的一些参数要求
            }
            LoggerManager::getInstance().AddLogger(logger);
            return logger;
        }
    };
}

#endif