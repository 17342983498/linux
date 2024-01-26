#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <jsoncpp/json/json.h>
#include "Util.hpp"

//#define MYSELF

namespace Protocaol_ns
{
    #define SEP " "
    #define SEP_LEN strlen(SEP)
    #define HEADER_SEP "\r\n"
    #define HEADER_SEP_LEN strlen("\r\n")

    //为了保证能够正确读到字符串，仅靠序列和反序列化是不够的，还需要再进一步对数据进行包装
    //而这个包装就是给有效载荷加报头  请求/响应 = 报头\r\n有效载荷\r\n

    //该方法在序列化之后执行
    std::string  AddHeader(const std::string& str)
    {
        std::string s = std::to_string(str.size());
        s += HEADER_SEP;
        s += str;
        s += HEADER_SEP;
        return s;
    }

    std::string RemoveHeader(const std::string &package, int len)
    {
        return package.substr(package.size() - len - HEADER_SEP_LEN, len);
    }

    int ReadPackage(int sock, std::string &inbuffer, std::string *package) //返回值-1代表对方已经关闭，不需要再读了，0代表得继续读，1代表读成功
    {
        //边读取
        char buffer[1024];
        ssize_t s = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (s < 0) return -1;
        buffer[s] = 0;
        inbuffer += buffer;

        //边分析
        auto pos = inbuffer.find(HEADER_SEP);
        if (pos == std::string::npos) return 0;
        std::string HeadStr = inbuffer.substr(0, pos); //获取了头部字符串
        int len = Util::toInt(HeadStr);
        int targetPackageLen = HeadStr.length() + len + 2 * HEADER_SEP_LEN; //inbuffer内容还未动，只是在分析
        if (inbuffer.size() < targetPackageLen) return 0;
        *package = inbuffer.substr(0, targetPackageLen); //提取整个字符串
        inbuffer.erase(0, targetPackageLen);
        return len;
    }

    class Request
    {
    public:
        Request()
        {}
        
        Request(int x, int y, char op) : _x(x), _y(y), _op(op)
        {}

        bool Serialize(std::string* outStr)
        {
#ifdef MYSELF
            std::string x_string = std::to_string(_x);
            std::string y_string = std::to_string(_y);

            *outStr = x_string + SEP + _op + SEP + y_string;
#else
            Json::Value root; // Value:一种万能对象，接受任意的KV类型
            root["x"] = _x; //会将传进来的所有都转变为字符串
            root["y"] = _y;
            root["op"] = _op;
            //Json::FastWriter writer; // writer 用来进行序列化 (struct->string)
            Json::StyledWriter writer;
            *outStr = writer.write(root);

#endif
            std::cout << "序列化：" << *outStr << std::endl;
            return true;
        }
        
        bool Deserialize(const std::string &inStr)
        {
#ifdef MYSELF
            std::vector<std::string> result;
            Util::StringSplit(inStr, SEP, &result);
            if (result.size() != 3) return false;
            if (result[1].size() != 1) return false;
            _x = Util::toInt(result[0]);
            _y = Util::toInt(result[2]);

            _op = result[1][0];
#else
            Json::Value root;
            Json::Reader reader;
            reader.parse(inStr, root);
            _x = root["x"].asInt();
            _y = root["y"].asInt();
            _op = root["op"].asInt();
#endif
            return true;
        }
        
        ~Request()
        {}
    //private:
        int _x;
        int _y;
        char _op;
    };

    class Response
    {
    public:
        Response()
        {}

        Response(int result, int code):_result(result), _code(code)
        {}
        
        bool Serialize(std::string* outStr)
        {
            *outStr = "";
#ifdef MYSELF
            std::string res_string = to_string(_result);
            string code_string = to_string(_code);
            *outStr = res_string + SEP + code_string;
#else
            Json::Value root;
            root["result"] = _result;
            root["code"] = _code;
            Json::FastWriter writer;
            *outStr = writer.write(root);
#endif
            std::cout << "序列化：" << *outStr << std::endl;
            return true;
        }
        
        bool Deserialize(const std::string& inStr)
        {
#ifdef MYSELF
            std::vector<std::string> result;
            Util::StringSplit(inStr, SEP, &result);
            if (result.size() != 2) return false;
            _result = Util::toInt(result[0]);
            _code = Util::toInt(result[1]);
#else
            Json::Value root;
            Json::Reader reader;
            reader.parse(inStr, root);
            _result = root["result"].asInt();
            _code = root["code"].asInt();
 
#endif
            return true;
        }
        ~Response()
        {}
    //private:
        int _result;
        int _code;//通过code代表不同的错误码
    };
}