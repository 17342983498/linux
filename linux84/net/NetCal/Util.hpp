#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Util
{
public:
    static bool StringSplit(const string &str, const string &sep, vector<string>*result)
    {
        size_t start = 0;
        while (start < str.size())
        {
            //找分隔符
            auto pos = str.find(sep, start);
            if (pos == string::npos)break;
            //切割字符串压入result中
            result->push_back(str.substr(start, pos - start));
            //定位下一个切割的起始位置
            start = pos + sep.size();
        }
        //防止字符串没找到分隔符而退出的循环
        if (start < str.size())
        {
            result->push_back(str.substr(start));
        }
        return true;
    }

    static int toInt(const std::string &s)
    {
        return atoi(s.c_str());
    }
};