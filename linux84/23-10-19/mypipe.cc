#include <iostream>
#include <cerrno>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>


int main()
{
    // 1. 创建管道
    int pipefd[2] = {0};
    int n = pipe(pipefd);
    if(n < 0)
    {
        std::cout << " pipe error, " << errno << ":" <<strerror(errno) << std::endl;
        return 1;
    }
    // 2. 创建子进程
    int fd = fork();
    if(fd < 0)
    {
        std::cout << "fork error, " << errno << ":" << strerror(errno) <<std::endl;
        return 1;
    }
    if(fd == 0)
    {
        //子进程
        close(pipefd[0]);
        std::string str = "你好，我是子进程";
        int cnt = 1;
        char buffer[1024] = {0};
        while(1)
        {
            snprintf(buffer,sizeof(buffer),"%s, 计数器:%d, ID:%d\n",str.c_str(),cnt++,getpid());
            write(pipefd[1],buffer,strlen(buffer));
            sleep(1);
        }
        close(pipefd[1]);
        exit(0);
    }

    //父进程
    // 3. 通信
    close(pipefd[1]);
    char buffer[1024] = {0};
    while(1)
    {
        int n = read(pipefd[0],buffer,sizeof(buffer)-1);
        if(n > 0)
        {
            buffer[n]='\0';
            std::cout << "我是父进程，子进程给我说：" << buffer << std::endl;
        }
    }
    close(pipefd[0]);
    return 0;
}