#include <iostream>
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "comm.hpp"

using namespace std;


int main()
{
    int wfd = open(filename.c_str(), O_WRONLY);
    if (wfd < 0)
    {
        cout << errno << " : " << strerror(errno) << endl;
        return 3;
    }

    char buffer[1024];
    while (true)
    {
        cout << "请输入您要发送的消息：" << endl;
        char* msg = fgets(buffer, sizeof(buffer), stdin);
        assert(msg);
        (void)msg;
        buffer[strlen(buffer) - 1] = 0;
        int n = write(wfd, buffer, strlen(buffer));
        assert(n);
        (void)n;
    }
    close(wfd);
    return 0;
}