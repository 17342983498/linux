#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


pid_t id=0;

void handler(int signo)
{
    sleep(5);
    printf("捕捉到一个信号：%d, who: %d\n",signo,getpid());
    while(1)
    {
        pid_t res = waitpid(-1, NULL, WNOHANG);
        if (res > 0)
        {
            printf("wait successful,res: %d, ID: %d\n", res, id);
        }
        else break;
    }
    printf("handler done\n");
}

int main()
{
    signal(SIGCHLD,handler);
    int i=1;
    for( ;i<=10;++i)
    {
        id = fork();
        if (id == 0)
        {
            // 子进程
            int cnt = 5;
            while (cnt)
            {
                printf("我是子进程, ID:%d, PPID:%d\n", getpid(), getppid());
                sleep(1);
                cnt--;
            }
            exit(1);
        }
    }

    while(true)
    {
        sleep(1);
    }
    return 0;
}