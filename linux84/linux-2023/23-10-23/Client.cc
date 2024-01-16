#include <iostream>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "comm.hpp"
using namespace std;

int main()
{
    int wfd = open(filename.c_str(), O_WRONLY);
    if (wfd < 0)
    {
        cerr << errno << ":" << strerror(errno) << endl;
        return 1;
    }
    char buffer[1024];
    while (true) 
    {
        cout << "请输入你的消息：";
        char* msg = fgets(buffer, sizeof(buffer), stdin);
        assert(msg);
        (void)msg;
        buffer[strlen(buffer) - 1] = 0;
        int n = write(wfd, buffer, strlen(buffer));
        assert(n);
        (void)n;
    }
    close(wfd);
}