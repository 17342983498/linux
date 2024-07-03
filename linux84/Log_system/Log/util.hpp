/*实用工具类的实现：
    1. 获取系统时间
    2. 判断文件是否存在
    3. 获取文件所在路径
    4. 创建目录
*/

#ifndef __M_UTIL_H__
#define __M_UTIL_H__

#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace LogSystem
{
    namespace util
    {
        class Date
        {
        public:
            static size_t getTime()
            {
                return (size_t)time(nullptr);
            }
        };

        class File
        {
        public:
            static bool exists(const std::string& pathname)
            {
                struct stat st;
                if (stat(pathname.c_str(), &st) < 0)
                {
                    return false;
                }
                return true;
                //return access(pathname.c_str(), F_OK);
            }
            static std::string path(const std::string& pathname)
            {
                size_t pos = pathname.find_last_of("/\\");
                if (pos == std::string::npos) return ".";
                return pathname.substr(0, pos + 1);
            }
            static void createDirectory(const std::string& pathname)
            {
                size_t pos = 0, idx = 0;
                while (idx < pathname.size())
                {
                    pos = pathname.find_first_of("/\\", idx);
                    if (pos == std::string::npos)
                    {
                        mkdir(pathname.c_str(), 0777);
                    }
                    std::string parent_dir = pathname.substr(0, pos + 1);
                    if(exists(parent_dir)==true)
                    {
                        idx = pos + 1;
                        continue;
                    }
                    mkdir(parent_dir.c_str(), 0777);
                    idx = pos + 1;
                }
            }
        };
    }
}
#endif