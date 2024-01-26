#pragma once

#include <iostream>
#include <pthread.h>
#include <functional>
#include "Sock.hpp"
#include "Protocaol.hpp"

using namespace Protocaol_ns;

namespace ns_tcpserver
{
    class TcpServer;
    using func_t = std::function<Response(const Request&)>;

    class ThreadData
    {
    public:
        ThreadData(int sock, std::string ip, uint16_t port, TcpServer* p) :_sock(sock), _clientip(ip), _clientport(port), _tsvrp(p)
        {}
        ~ThreadData()
        {}
    
        int _sock;
        std::string _clientip;
        uint16_t _clientport;
        TcpServer* _tsvrp;
    };



    class TcpServer
    {
    public:
        TcpServer(func_t func, uint16_t port) :_func(func), _port(port)
        {}
        void InitServer()
        {
            //初始化服务器
            _listensock.Socket();
            _listensock.Bind(_port);
            _listensock.Listen();
            logMessage(Info, "Init server done, listensock: %d", _listensock.Fd());
        }

        void ServiceIO(int sock, const std::string clientip, const uint16_t clientport)
        {
            // 1. 读到一个字符串，可是你能确定读到的就是正确的字符串吗，不能，因为TCP是字节流的，如果过不能正确的分割就读不到正确的的字符串
            // 因此还需要对协议（序列和反序列化）做进一步要求
            //循环读取，边读取，边检测
            while (true)
            {
                std::string inbuffer; // 用来储存客户端发来的所有数据
                std::string package; //原始报文
                // "7"\r\n"10 + 20"\r\n
                int n = ReadPackage(sock, inbuffer, &package);
                if (n == -1)
                    break;
                else if (n == 0)
                    continue;
                else
                {
                    // 2. 这里已经提取到了整个有效字符串，再提取有效载荷（有效载荷）
                    std::string payload = RemoveHeader(package, n);

                    // 3. 反序列化
                    Request req;
                    req.Deserialize(payload);
                    // 4. 计算
                    Response resp = _func(req);
                    // 5. 将计算结果序列化
                    std::string send_string;
                    resp.Serialize(&send_string);
                    // 6.添加报头
                    send_string = AddHeader(send_string);
                    // 7.发送
                    send(sock, send_string.c_str(), send_string.size(), 0); 
                }
            }
            close(sock);
        }

        static void* ThreadRoutine(void* args)
        {
            pthread_detach(pthread_self());
            ThreadData* td = static_cast<ThreadData *>(args);
            td->_tsvrp->ServiceIO(td->_sock, td->_clientip, td->_clientport);
            logMessage(Debug, "client quit ...");
            delete td;
            return nullptr;
        }
        
        void Start()
        {
            for (;;)
            {
                std::string clientip;
                uint16_t clientport;
                int sock = _listensock.Accept(&clientip, &clientport);
                while (sock < 0) continue;
                logMessage(Debug, "get a new client, client info : [%s:%d]", clientip.c_str(), clientport);

                pthread_t tid;
                ThreadData* td = new ThreadData(sock, clientip, clientport, this);
                pthread_create(&tid, nullptr, ThreadRoutine, td);
            }
        }
        ~TcpServer()
        {
            _listensock.Close();
        }
    private:
        uint16_t _port;
        Sock _listensock;
        func_t _func;
    };
};