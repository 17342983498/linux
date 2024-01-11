#pragma once

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <functional>

#include "err.hpp"


using namespace std;


namespace ns_server
{
    static const uint16_t defaultport = 8848;
    static const int backlog = 64;
    using func_t = function<string(const string)>;

    class TcpServer
    {
    public:


        TcpServer(func_t func, uint16_t port = defaultport) :func_(func), port_(port), state_(false)
        {
            
        }
        void InitServer()
        {
            // 1. 创建socket
            listensock_ = socket(AF_INET, SOCK_STREAM, 0);
            if (listensock_ < 0)
            {
                cerr << "create socket error: " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }
            cout << "create socket success: " << listensock_ << endl;

            // 2. bind
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(port_);
            local.sin_addr.s_addr = htonl(INADDR_ANY);

            if (bind(listensock_, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                cerr << "bind socket error " << strerror(errno) << endl;
                exit(BIND_ERR);
            }
            cout << "bind socket success: " << listensock_ << endl;

            // 3. 监听
            if (listen(listensock_, backlog) < 0)
            {
                cerr << "listen socket error - " << strerror(errno) << endl;
                exit(LISTEN_ERR);
            }
            cout << "listen socket success: " << listensock_ << endl;
        }
        void Start()
        {
            state_ = true; //表示服务器已启动
            while (state_)
            {
                // 4. 获取连接，accept
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int sock = accept(listensock_, (struct sockaddr*)&client, &len);
                if (sock < 0)
                {
                    cerr << "listen socket error - " << strerror(errno) << endl;
                    continue;
                }

                uint16_t clientport = ntohs(client.sin_port);
                string clientip = inet_ntoa(client.sin_addr);//该函数直接将字符串风格IP转化为四字节IP
                cout << "accept socket success: " << sock << " from " << listensock_ << " | " << clientip << " - " << clientport << endl;

                pid_t id = fork();
                if (id < 0)
                {
                    close(sock);
                    continue;
                }
                else if (id == 0) //子进程将不需要的套接字
                {
                    close(listensock_);
                    if (fork() > 0) exit(0);//让子进程再创子进程（孙子进程），成功后子进程退出，孙子进程往下执行，最后会变成孤儿进程被系统回收 
                    service(sock, clientip, clientport);
                    exit(0);
                }

                close(sock);//父进程一定要关闭不需要的fd，不关闭会导致fd泄露
                pid_t ret = waitpid(id, nullptr, 0);
                if (ret == id)
                {
                    cout << " wait child " << id << " success " << endl;
                }

                //service(sock, clientip, clientport);
            }
        }

        void service(int sock, string &clientip, uint16_t &clientport)
        {
            char buffer[1024];
            while (true)
            {
                ssize_t s = read(sock, &buffer, sizeof(buffer) - 1);
                if (s > 0)
                {
                    buffer[s] = 0;
                    string res = func_(buffer); //回调
                    cout << clientip << " - " << clientport << " # " << res << endl;
                    write(sock, res.c_str(), res.size());
                }
                else if(s == 0)
                {
                    //对方已经将连接断开
                    close(sock);
                    cout << clientip << " - " << clientport << " quit! " << endl;
                    break;
                }
                else
                {
                    close(sock);
                    cerr << "read error - " << strerror(errno) << endl;
                    break;
                }
            }
        }

        ~TcpServer()
        {
            
        }
    private:
        int listensock_;
        uint16_t port_;
        bool state_;
        func_t func_;
    };
};