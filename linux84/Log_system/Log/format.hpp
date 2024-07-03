#ifndef __M_FMT_H__
#define __M_FMT_H__

#include "level.hpp"
#include "message.hpp"
#include <vector>
#include <cassert>
#include <sstream>
#include <ctime>


namespace LogSystem
{
    // 抽象格式化子项基类
    class FormatItem
    {
    public:
        using ptr = std::shared_ptr<FormatItem>;
        virtual void format(std::ostream& out, const LogMessage& msg) = 0;
    };

    class MsgFormatItem :public FormatItem  //日志消息
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << msg._payload;
        }
    };
    
    class LevelFormatItem :public FormatItem  //日志等级
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << LogLevel::toString(msg._level);
        }
    };

    class TimeFormatItem :public FormatItem  //时间
    {
    public:
        TimeFormatItem(const std::string& fmt = "%H:%M:%S") :_time_fmt(fmt) {}
        void format(std::ostream& out, const LogMessage& msg)override
        {
            struct tm t;
            localtime_r(&msg._ctime, &t);
            char tmp[32] = { 0 };
            strftime(tmp, 31, _time_fmt.c_str(), &t);
            out << tmp;
        }
    private:
        std::string _time_fmt;
    };

    class FileFormatItem :public FormatItem  
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << msg._file;
        }
    };

    class LineFormatItem :public FormatItem
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << msg._line;
        }
    };

    class ThreadFormatItem :public FormatItem
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << msg._tid;
        }
    };

    class LoggerFormatItem :public FormatItem  //日志器
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << msg._logger;
        }
    };

    class TabFormatItem :public FormatItem  //制表符
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << "\t";
        }
    };

    class NewLineFormatItem :public FormatItem  //换行
    {
    public:
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << "\n";
        }
    };

    class OtherFormatItem :public FormatItem  //其他
    {
    public:
        OtherFormatItem(const std::string& str) :_str(str) {}
        void format(std::ostream& out, const LogMessage& msg)override
        {
            out << _str;
        }
    private:
        std::string _str;
    };

    class Formatter
    {
    public:
        using ptr = std::shared_ptr<Formatter>;
    
        //在这个构造函数内首先会对格式化模式进行确定，一旦好之后就需调用createItem函数
        //根据该模式完成每一个item对象的构造并放入items数组中，之后便根据这个数组中的每个对象调用format函数完成消息组织
        Formatter(const std::string& pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n") :_pattern(pattern)
        {
            assert(parsePattern());
        }

        void format(std::ostream& out, const LogMessage &msg)
        {
            for(auto &item:_items)
            {
                item->format(out, msg);
            }
        }

        std::string format(const LogMessage& msg)
        {
            std::stringstream ss;
            format(ss, msg);
            return ss.str();
        }
    private:
        //对格式化字符串进行解析
        bool parsePattern()
        {
            std::vector<std::pair<std::string, std::string>> fmt_order;
            size_t pos = 0;
            std::string key, val;
            while (pos < _pattern.size())
            {
                if (_pattern[pos] != '%') //处理原始字符串--判断是否为%，不是的话就是原始字符
                {
                    val.push_back(_pattern[pos++]);
                    continue;
                }
                //走到这里代表上一个字符是%
                if (pos + 1 < _pattern.size() && _pattern[pos + 1] == '%')//判断%后面是否还跟着一个%
                {
                    val.push_back('%');
                    pos += 2;
                    continue;
                }
                //走到这里代表原始字符串已经处理完毕
                if(val.empty() == false)
                {
                    fmt_order.push_back(std::make_pair("", val));
                    val.clear();
                }
                //此时pos指向%
                pos += 1;//代表指向%后的格式化字符
                if (pos == _pattern.size())
                {
                    std::cout << "在%之后，没有对应的格式化字符！" << std::endl;
                    return false;
                }
                key = _pattern[pos];
                pos += 1;
                //探究时间格式化字符串
                if (pos < _pattern.size() && _pattern[pos] == '{')
                {
                    pos += 1;
                    while (pos < _pattern.size() && _pattern[pos] != '}')
                    {
                        val.push_back(_pattern[pos++]);
                    }
                    if (pos == _pattern.size())
                    {
                        std::cout << "子规则{}匹配出错！"<< std::endl;
                        return false;
                    }
                    pos += 1;
                }
                fmt_order.push_back(std::make_pair(key, val));
                key.clear();
                val.clear();
            }
            //根据解析得到的数据初始化格式化子项数组成员
            for (auto& it : fmt_order)
            {
                _items.push_back(createItem(it.first, it.second));
            }
            return true;
        }

        FormatItem::ptr createItem(const std::string& key, const std::string& val)
        {
            if (key == "d") return std::make_shared<TimeFormatItem>(val);
            if (key == "t") return std::make_shared<ThreadFormatItem>();
            if (key == "c") return std::make_shared<LoggerFormatItem>();
            if (key == "f") return std::make_shared<FileFormatItem>();
            if (key == "l") return std::make_shared<LineFormatItem>();
            if (key == "p") return std::make_shared<LevelFormatItem>();
            if (key == "T") return std::make_shared<TabFormatItem>();
            if (key == "m") return std::make_shared<MsgFormatItem>();
            if (key == "n") return std::make_shared<NewLineFormatItem>();
            return std::make_shared<OtherFormatItem>(val);
        }
    private:
        std::string _pattern;
        std::vector<FormatItem::ptr> _items;
    };
}


#endif