#ifndef __M_LOG_SYS_H__
#define __M_LOG_SYS_H__

#include "logger.hpp"

namespace LogSystem
{

    // 提供获取指定日志器的全局接口，防止用户自己操作单例对象
    Logger::ptr GetLogger(const std::string &name)
    {
        return LogSystem::LoggerManager::getInstance().GetLogger(name);
    }

    Logger::ptr RootLogger()
    {
        return LogSystem::LoggerManager::getInstance().RootLogger();
    }

// 使用宏函数对日志器的接口进行代理（代理模式）
#define debug(fmt, ...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define info(fmt, ...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define warn(fmt, ...) warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define error(fmt, ...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define fatal(fmt, ...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__);

// 提供宏函数，直接通过默认日志器进行日志dde标准输出打印
#define DEBUG(fmt, ...) LogSystem::RootLogger()->debug(fmt, ##__VA_ARGS__);
#define INFO(fmt, ...) LogSystem::RootLogger()->info(fmt, ##__VA_ARGS__);
#define WARN(fmt, ...) LogSystem::RootLogger()->warn(fmt, ##__VA_ARGS__);
#define ERROR(fmt, ...) LogSystem::RootLogger()->error(fmt, ##__VA_ARGS__);
#define FATAL(fmt, ...) LogSystem::RootLogger()->fatal(fmt, ##__VA_ARGS__);

}

#endif