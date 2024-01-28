#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Log.hpp"
#include "ERR.hpp"

static const int gbacklog = 32;
static const int defaultfd = -1;

class Sock
{
public:
    Sock():_sock(defaultfd)
    {}
    void Socket()
    {
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_sock < 0)
        {
            logMessage(Fatal, "socket error, code: %d, errstring: %s", errno, strerror(errno));
            exit(SOCKET_ERR);
        }
    }
    void Bind(uint16_t &port)
    {
        struct sockaddr_in local;
        memset(&local, sizeof(local), 0);
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            logMessage(Fatal, "bind error, code: %d, errstring: %s", errno, strerror(errno));
            exit(BIND_ERR);
        }
    }

    //----------------server-----------------
    void Listen()
    {
        if (listen(_sock, gbacklog) < 0)
        {
            logMessage(Fatal, "listen error, code: %d, errstring: %s", errno, strerror(errno));
            exit(LISTEN_ERR);
        }
    }
    int Accept(std::string *clientip, uint16_t *clientport)
    {
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        int sock = accept(_sock, (struct sockaddr*)&temp, &len);
        if (sock < 0)
        {
            logMessage(Warning, "accept error, code: %d, errstring: %s", errno, strerror(errno));
        }
        else
        {
            *clientip = inet_ntoa(temp.sin_addr);
            *clientport = ntohs(temp.sin_port);
        }
        return sock;
    }

    //---------------client---------------
    int Connect(const std::string& serverip, uint16_t& serverport)
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(serverport);
        server.sin_addr.s_addr = inet_addr(serverip.c_str());
        return connect(_sock, (struct sockaddr*)&server, sizeof(server));
    }
    int Fd()
    {
        return _sock;
    }

    void Close()
    {
        if (_sock != defaultfd)
        {
            close(_sock);
        }
    }

    ~Sock()
    {
    }
private:
    int _sock;
};