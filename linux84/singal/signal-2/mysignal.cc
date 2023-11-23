#include <iostream>
#include <unistd.h>
#include <cassert>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

// void handler()
// {

// }

void showBlock(sigset_t* oset)
{
    int n = 1;
    for (;n <= 31;n++)
    {
        if (sigismember(oset, n))
            cout << 1;
        else
            cout << 0;
    }
    cout << endl;
}

void showPending(sigset_t* pending)
{
    int signo = 1;
    for (;signo <= 31;signo++)
    {
        if (sigismember(pending, signo))
            cout << 1;
        else
            cout << 0;
    }
    cout << endl;
}

void handler(int signo)
{
    cout << "2号信号被送达且已经被处理" << endl;
    exit(1);
}


int main()
{
    signal(SIGINT, handler);


    sigset_t set, oset;
    sigemptyset(&set);
    sigemptyset(&oset);
    sigaddset(&set, 2);

    //对2号信号进行阻塞，保证进程收到2号信号的时候会处于未决状态
    sigprocmask(SIG_SETMASK, &set, &oset);
    int cnt = 0;

    while (true)
    {
        sigset_t pending;
        sigemptyset(&pending);
        //对进程的pendign表进行获取，在收到2号信号时，对应比特位被设为1
        int n = sigpending(&pending);
        assert(n == 0);
        (void)n;

        showPending(&pending);
        sleep(1);

        cnt++;
        //在10秒之后对2号信号解除
        if (cnt == 10)
        {
            cout << "解除了对2号信号的阻塞" << endl;
            sigprocmask(SIG_SETMASK, &oset, nullptr);
            //但是在10秒之前对进程发送了2号信号的话，十秒之后2号信号就会立即送达，进程终止。
        }
    }






    // while (true)
    // {
    //     showBlock(&oset);
    //     //showBlock(&set);
    //     cnt++;
    //     sleep(1);
    //     if (cnt == 10)
    //     {
    //         sigprocmask(SIG_SETMASK, &oset, &set);
    //         showBlock(&set);
    //     }
    // }


    // signal(2, SIG_IGN);
    // while (1)
    // {}
    

    // pid_t id = fork();
    // if (id == 0)
    // {
    //     sleep(2);
    //     cout << "野指针问题...here" << endl;
    //     cout << "野指针问题...here" << endl;
    //     int* p = nullptr;
    //     *p = 10;
    //     cout << "野指针问题...here" << endl;
    //     cout << "野指针问题...here" << endl;
    // }
    // int status = 0;
    // waitpid(id, &status, 0);
    // cout << "exit code:" << ((status >> 8) & 0xff) << endl;
    // cout << "exit signal:" << (status & 0x7f) << endl;
    // cout << "code dump flag:" << ((status >> 7) & 0x1) << endl;
    return 0;
}