#include "common.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    //1.先创建管道，只需建一次，客户端只需打开这个管道就行
    umask(0);
    int n = mkfifo(fifoname.c_str(), mode);
    if(n!=0)
    {
        std::cout<<errno<<":"<<strerror(errno)<<std::endl;
        return 1;
    }

    //2.服务端开启管道文件
    std::cout << "create fifo file success" << std::endl;
    int rfd = open(fifoname.c_str(),O_RDONLY);
    if(rfd<0)
    {
        std::cout<<errno<<":"<<strerror(errno)<<std::endl;
        return 2;
    }
    std::cout << "open fifo success, begin ipc" << std::endl;

    //3.正常通信
    char buffer[NUM];
    while(true)
    {
        buffer[0]=0;
        ssize_t n =read(rfd,buffer,sizeof(buffer)-1);
        if(n>0)
        {
            buffer[n]=0;
            //std::cout<<"client# "<<buffer<<std::endl;
            printf("%c",buffer[0]);
            fflush(stdout);
        }
        else if(n == 0)
        {
            std::cout << "client quit, me too" << std::endl;
            break;
        }
        else 
        {
            std::cout << errno << " : " << strerror(errno) << std::endl;
            break;
        }
    }

    //4.关闭
    close(rfd);
    unlink(fifoname.c_str());
    return 0;
}
