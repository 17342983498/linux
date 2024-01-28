#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include "Log.hpp"


class Util
{
public:
    static bool ReadFile(const std::string path, std::string* fileContent)
    {
        // 1. 获取文件本身大小
        struct stat st;
        int n = stat(path.c_str(), &st);
        if (n < 0) return false;
        int size = st.st_size;
        // 2. 调整string空间
        fileContent->resize(size);
        // 3. 读取
        int fd = open(path.c_str(), O_RDONLY);
        if (fd < 0) return false;
        read(fd, (char*)fileContent->c_str(), size);
        close(fd);
        logMessage(Info, "read file %s done", path.c_str());
        return true;
    }
    static std::string ReadOneLine(std::string &message, const std::string &sep)
    {
        auto pos = message.find(sep);
        if (pos == std::string::npos) return "";
        std::string s = message.substr(0, pos);
        message.erase(0, pos + sep.size());
        return s;
    }
    static bool ParaRequestLine(std::string& line, std::string* _method, std::string* _url, std::string* _httpVersion)
    {
        std::stringstream ss(line);
        ss >> *_method >> *_url >> *_httpVersion;
        return true;
    }
};