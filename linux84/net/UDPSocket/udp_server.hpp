#pragma once

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>      
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include "err.hpp"
#include <functional>
#include <stdio.h>
#include <unordered_map>
#include "RingQueue.hpp"
#include "LockGuard.hpp"
#include "mythread.hpp"
#include <pthread.h>

using namespace std;


namespace NS_SERVER
{
    const static uint16_t default_port = 8888;
    using func_t = function<string(string)>;

    class UdpServer
    {
    public:    
        // UdpServer(func_t cb, uint16_t port = default_port):service_(cb), port_(port)
        UdpServer(uint16_t port = default_port):port_(port)
        {
            pthread_mutex_init(&lock_, nullptr);
            p = new Thread(1, bind(&UdpServer::Recv, this));
            c = new Thread(2, bind(&UdpServer::Broadcast, this));
        }

        void InitServer()
        {
            // 1. 创建套接字,打开网络文件
            sock_ = socket(AF_INET, SOCK_DGRAM, 0);
            if (sock_ < 0)
            {
                cerr << "create socket error: " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }
            cout << "create socket success: " << sock_ << endl;

            // 2. 给服务器指定port，标识将来客户端发送数据的目标
            struct sockaddr_in local;
            bzero(&local, sizeof(local));

            local.sin_family = AF_INET;
            local.sin_port = htons(port_);
            //要将字符串风格的IP地址转化成4字节int，需要将主机序列转化为网络序列
            //云服务器或者一般服务器一般不要指明某一个确定的IP
            local.sin_addr.s_addr = INADDR_ANY;//让我们的udpserver在启动的时候，bind本主机上的任意IP

            if (bind(sock_, (struct sockaddr*)&local, sizeof(local)) < 0)
            {
                cerr << "bind socket error " << strerror(errno) << endl;
                exit(BIND_ERR);
            }
            cout << "bind socket success: " << sock_ << endl;

            p->run();
            c->run();
        }

        void addUser(const string name, struct sockaddr_in peer)
        {
            LockGuard lg(&lock_);
            auto iter = onlineuser_.find(name);
            if (iter != onlineuser_.end())
            {
                return;
            }
            onlineuser_.insert(pair<string, struct sockaddr_in>(name, peer));
        }

        void Recv()
        {
            char buffer[1024];
            while (true)
            {
                struct sockaddr_in peer;//表示客户端的sockaddr结构
                socklen_t length = sizeof(peer);
                //收
                int n = recvfrom(sock_, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &length);
                if (n > 0)
                {
                    buffer[n] = '\0';
                }
                else
                    continue;
                
                //提取client信息
                uint16_t clientport = ntohs(peer.sin_port);
                string clientip = inet_ntoa(peer.sin_addr);//该函数直接将字符串风格IP转化为四字节IP
                cout << clientip << " - " << clientport << "# " << buffer << endl;

                string name = clientip;
                name += "-";
                name += to_string(clientport);

                addUser(name, peer);//构建客户端并记录

                string message = name + " >> " + buffer;
                
                rq_.push(message);

                //string response = service_(buffer);

                //发
                //sendto(sock_, response.c_str(), response.size(), 0, (struct sockaddr*)&peer, sizeof(peer));
            }
        }

        void Broadcast()
        {
            while (true) 
            {
                string sendstring;
                rq_.pop(&sendstring);
                vector<struct sockaddr_in> v;
                {
                    LockGuard lg(&lock_);
                    for (auto user : onlineuser_)
                        v.push_back(user.second);
                }
                for (auto user : v)
                {
                    //cout << "broadcast message to " << user.first <<" "<< sendstring << endl;
                    cout << "send done ..." << sendstring << endl;
                    sendto(sock_, sendstring.c_str(), sendstring.size(), 0, (struct sockaddr*)&user, sizeof(user));
                }
            }
        }

        ~UdpServer()
        {
            pthread_mutex_destroy(&lock_);
            c->join();
            p->join();
            delete c;
            delete p;
        }
    private:
        int sock_;
        uint16_t port_;
        func_t service_;
        unordered_map<string, struct sockaddr_in> onlineuser_;
        pthread_mutex_t lock_;
        RingQueue<string> rq_;
        Thread *c, *p;
    };
}