#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/types.h>      
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "err.hpp"
#include <pthread.h>

using namespace std;

static void usage(string proc)
{
    cout << "Usage\t\t" << proc << " serverip serverprot\n" << endl;
}

void *Recv(void *args)
{
    int sock = *(static_cast<int*>(args));
    while (true)
    {
        char buffer[2048];
        struct sockaddr_in temp;
        socklen_t length = sizeof(temp);
        int  n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&temp, &length);
        if (n > 0)
        {
            buffer[n] = '\0';
            cout << buffer << endl;
            // cout << "server echo# " << buffer << endl;
        }
    }
    return nullptr;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }

    string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        cerr << "create socket error: " << strerror(errno) << endl;
        exit(SOCKET_ERR);
    }
    cout << "create socket success: " << sock << endl;

    //明确server
    struct sockaddr_in server;
    memset(&server, 0 ,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(serverip.c_str());
    server.sin_port = htons(serverport);

    pthread_t tid;
    pthread_create(&tid, nullptr, Recv, &sock);

    while (true)
    {
        string message;
        cerr << "client Enter# ";
        //cin >> message;
        getline(cin, message);

        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));
    }

    return 0;
}