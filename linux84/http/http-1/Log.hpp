#pragma once

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstdarg>
#include <stdio.h>


enum
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal,
    Uknown
};

static std::string tolevelString(int level)
{
    switch (level)
    {
        case Debug:
            return "Debug";
        case Info:
            return "Info";
        case Warning:
            return "Warning";
        case Error:
            return "Error";
        case Fatal:
            return "Fatal";
        default:
            return "Uknown";

    }
}

static std::string getTime()
{
    time_t curr = time(nullptr);
    struct tm* tmp = localtime(&curr);
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%d-%d-%d %d:%d:%d", tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
    return buffer;
}

//日志格式：日志等级 时间 pid 消息体
void logMessage(int level, const char *format, ...)
{
    char logLeft[1024];
    std::string levelString = tolevelString(level);
    std::string timeString = getTime();
    snprintf(logLeft, sizeof(logLeft), "%s [%s] [%d] ", levelString.c_str(), timeString.c_str(), getpid());

    char logRight[1024];
    va_list p;
    va_start(p, format);
    vsnprintf(logRight, sizeof(logRight), format, p);
    va_end(p);

    printf("%s%s\n", logLeft, logRight);
}
