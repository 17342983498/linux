#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>

int count=0;

void Usage(std::string proc)
{
    std::cout<<"\t Usage: \n\t";
    std::cout<<proc<<" 信号编号 目标进程ID\n"<<std::endl;
}

void handler(int signum)
{
    //std::cout<<"count:"<<count<<" "<<signum<<std::endl;
    std::cout << "get a signal: " << signum << " count: " << count << std::endl;
    int n = alarm(10);
    std::cout << "return: " << n << std::endl; 
    //exit(signum);
}

int main(int argc,char* argv[])
{
    std::cout<<getpid()<<std::endl;
    signal(14,handler);
    alarm(10);
    while(1)
    {
        //count++;
        //sleep(1);
    }


/*     if(argc!=3)
    {
        Usage(argv[0]);
        exit(1);
    }

    int signum=atoi(argv[1]);
    int targret_ID=atoi(argv[2]);
    int n = kill(targret_ID,signum);
    if(n==-1)
    {
        std::cerr<<errno<<":"<<strerror(errno)<<std::endl;
        exit(2);
    } */

    return 0;
}

/* void handler(int signum)
{
    std::cout<<"我是自定义信号捕捉函数:"<<signum<<std::endl;
}

int main()
{
    signal(2,handler);

    while(1)
    {
        std::cout<< "我正在运行,我的pid是:"<<getpid()<<std::endl;
        sleep(1);
    }
    return 0;
} */