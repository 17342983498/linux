#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "err.hpp"

using namespace std;


static void usage(string proc)
{
    cout << "Usage\t\t" << proc << " serverip serverprot\n" << endl;
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

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cerr << "create socket error: " << strerror(errno) << endl;
        exit(SOCKET_ERR);
    }
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    inet_aton(serverip.c_str(), &server.sin_addr);
    server.sin_port = htons(serverport);

    int cnt = 5;

    while (connect(sock, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
        cout << "尝试重连中，重连次数还剩：" << cnt-- << endl;
        sleep(1);
        if (cnt <= 0)
        {
            break;
        }
    }

    if (cnt <= 0)
    {
        cerr << "连接失败" << endl;
        exit(CONNECT_ERR);
    }
    char buffer[1024];
    while (true)
    {
        string message;
        cout << "client enter# ";
        getline(cin, message);
        write(sock, message.c_str(), message.size());

        ssize_t s = read(sock, &buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0;
            cout << "server echo# " << buffer << endl;
        }
        else if (s == 0)//读失败
        {
            cerr << "server quit" << endl;
            break;
        }
        else //服务器挂
        {
            cerr << "readerror: " << strerror(errno) << endl;
            break;
        }
    }
    close(sock);
    return 0;
}