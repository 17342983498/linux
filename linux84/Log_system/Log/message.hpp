#ifndef __M_NSG_H__
#define __M_NSG_H__


#include <iostream>
#include <thread>
#include <string>
#include "level.hpp"
#include "util.hpp"

namespace LogSystem
{
    struct LogMessage
    {
        time_t _ctime;//日志生成时间
        LogLevel::value _level;//日志等级
        size_t _line;//行号
        std::thread::id _tid;//线程ID
        std::string _file;//源文件名
        std::string _logger;//日志器名称
        std::string _payload;//有效信息数据

        LogMessage(LogLevel::value level,
            size_t line,
            const std::string file,
            const std::string logger,
            const std::string payload) :
            _ctime(util::Date::getTime()),
            _level(level),
            _line(line),
            _tid(std::this_thread::get_id()),
            _file(file),
            _logger(logger),
            _payload(payload)
        {}
    };
}

#endif