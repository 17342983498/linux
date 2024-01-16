#include <iostream>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
    //第一步：创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1)
    {
        perror("创建管道失败！！！！\n");
    }

    //第二步：创建子进程
    pid_t id = fork();
    if(id<0)
    {
        perror("创建子进程失败！！！\n");
    }
    //子进程
    else if(id==0)
    {
        //第三步：关闭不必要的fd
        close(pipefd[0]);
        const std::string str="hello i am child !!!";
        int cnt=1;
        char buf[1024]={0};
        while(true)
        {
            snprintf(buf,sizeof buf,"%s, 计数器: %d, 我的pid: %d",str.c_str(),cnt++,getpid());
            write(pipefd[1],buf,sizeof(buf));
            sleep(1);
        }
        close(pipefd[1]);
        exit(-1);
    }
    //父进程
    close(pipefd[1]);
    char buf[1024]={0};
    while(true)
    {
        //sleep(1);
        read(pipefd[0],buf,sizeof(buf));
        std::cout<<"i am father, child give me message: "<<buf<<std::endl;
    }
    close(pipefd[0]);

    return 0;
}