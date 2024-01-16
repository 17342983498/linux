#include <iostream>
#include <signal.h>
#include <cassert>
#include <cstring>
#include <unistd.h>
using namespace std;


static void Printpending(const sigset_t &pending)
{
    cout << "当前进程的pending位图: ";
    for(int signo=1;signo<=31;++signo)
    {
        if(sigismember(&pending,signo))
            cout<<"1";
        else
            cout<<"0";
    }
    cout<<endl;
}

void handler(int signo)
{
    cout << "对特定信号："<< signo << "执行捕捉动作" << endl;
}

int main()
{
    int cnt=0;
    signal(2, handler);
    //1. 屏蔽2号信号
    sigset_t set,oset;
    //1.1 初始化
    sigemptyset(&set);
    sigemptyset(&oset);
    //1.2 将2号信号添加到set中
    sigaddset(&set,2);
    //1.3 将新的信号屏蔽字设置到进程中
    sigprocmask(SIG_BLOCK,&set,&oset);

    //2. while获取进程的pending信号集合，并01打印
    while(true)
    {
        sigset_t pending;
        sigemptyset(&pending);
        //获取pending表
        int n = sigpending(&pending);
        assert(n==0);
        (void)n;
        
        //打印pending集
        Printpending(pending);
        sleep(1);
        if(cnt++==10)
        {
            cout << "解除对2号信号的屏蔽" << endl;
            sigprocmask(SIG_SETMASK,&oset,nullptr);
        }
    }
    return 0;
}