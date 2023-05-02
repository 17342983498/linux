#include <iostream>
using namespace std;
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void showBlock(sigset_t *oset)
{
    int signo = 1;
    for(; signo <=31; signo++)
    {
        if(sigismember(oset, signo)) cout << "1";
        else cout << "0";
    }
    cout << endl;
}

void handler(int signo)
{
    cout << "我们的进程确实是收到了: " << signo << " 信号导致崩溃的" << endl;
    //exit(1);
}

int main()
{
    // sigset_t set,oset;
    // sigemptyset(&set);
    // sigemptyset(&oset);
    // sigaddset(&set, 2); //SIGINT

    // // 设置进入进程，谁调用，设置谁
    // sigprocmask(SIG_SETMASK, &set, &oset); // 1. 2号信号没有反应 2. 我们看到老的信号屏蔽字block位图是全零、
    // int cnt = 0;
    // while(true)
    // {
    //     showBlock(&oset);
    //     sleep(1);
    //     cnt++;

    //     if(cnt == 10)
    //     {
    //         cout << "recover block" << endl;
    //         sigprocmask(SIG_SETMASK, &oset, &set);
    //         showBlock(&set); 
    //     }
    // }

    signal(2, SIG_DFL);
    //signal(2, SIG_IGN);

    // while(true)
    // {
    //     sleep(1);
    // }

    // pid_t id = fork();
    // if (id == 0)
    // {
    //     sleep(2);
    //     cout << "野指针问题 ... here" << endl;
    //     cout << "野指针问题 ... here" << endl;
    //     cout << "野指针问题 ... here" << endl;

    //     int *p = nullptr;
    //     *p = 100; // 2, 野指针问题

    //     cout << "野指针问题 ... here" << endl;
    //     cout << "野指针问题 ... here" << endl;
    //     cout << "野指针问题 ... here" << endl;

    //     exit(0);
    // }
    // int status = 0;
    // waitpid(id, &status, 0);
    // cout << "exit code: " << ((status>>8) & 0xFF) << endl;
    // cout << "exit signal: " << (status & 0x7F) << endl;
    // cout << "core dump flag: " << ((status>>7 & 0x1)) << endl;


    //signal(2,handler);
    signal(2, SIG_DFL);

    while(true)
    {
        sleep(1);
    }

    return 0;
}