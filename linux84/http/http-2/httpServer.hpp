#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <functional>
#include "Sock.hpp"


static const uint16_t defaultport = 8848;

class httpServer;
     

using func_t = std::function<std::string(std::string &)>;




class ThreadData
{
public:
    ThreadData(int sock, std::string ip, uint16_t port, httpServer* p) :_sock(sock), _clientip(ip), _clientport(port), _tsvrp(p)
    {}
    ~ThreadData()
    {}

    int _sock;
    std::string _clientip;
    uint16_t _clientport;
    httpServer* _tsvrp;
};


class httpServer
{
public:
    httpServer(func_t func, uint16_t port = defaultport):_func(func), _port(port)
    {}

    void InitServer()
    {
        _listensock.Socket();
        _listensock.Bind(_port);
        _listensock.Listen();
    }

    void httpRequest(int sock)
    {
        char buffer[4096];
        std::string request;
        ssize_t s = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (s > 0)
        {
            buffer[s] = 0;
            request = buffer;
            std::string response = _func(request);
            send(sock, response.c_str(), response.size(), 0);
        }
        else
        {
            logMessage(Info, "client quit...");
        }
    }

    static void* threadRoutine(void* args)
    {
        pthread_detach(pthread_self());
        ThreadData* td = static_cast<ThreadData*>(args);
        td->_tsvrp->httpRequest(td->_sock);
        close(td->_sock);
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
            if (sock < 0) continue;

            pthread_t tid;
            ThreadData *td = new ThreadData(sock, clientip, clientport, this);
            pthread_create(&tid, nullptr, threadRoutine, td);
        }
    }

    ~httpServer()
    {}
private:
    uint16_t _port;
    Sock _listensock;
    func_t _func;
};


