#include "common.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    int wfd=open(fifoname.c_str(),O_WRONLY);
    if(wfd<0)
    {
        std::cout<<errno<<":"<<strerror(errno)<<std::endl;
        return 3;
    }

    char buffer[NUM];
    while(true)
    {
/*         std::cout << "请输入你的消息# ";
        char *msg = fgets(buffer, sizeof(buffer), stdin);
        assert(msg);
        (void)msg;
        buffer[strlen(buffer) - 1] = 0;       
        ssize_t n = write(wfd, buffer, strlen(buffer)); */

        system("stty raw");
        int c = getchar();
        system("stty -raw");
        if(c==0)
        {
            break;
        }

        ssize_t n = write(wfd, (char*)&c, sizeof(char)); 
        assert(n >= 0);
        (void)n;
    }
    close(wfd);
}