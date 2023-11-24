#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

volatile int quit = 0;

pid_t id;

void handler(int signo)
{
    sleep(5);
    cout << "i am father, my pid is " << getpid() << endl;

    while (1)
    {
        pid_t ret = waitpid(-1, NULL, 0);
        if (ret > 0)
        {
            cout << "子进程已经被收回" << " " << ret << " " << id << endl;
        }
        else
            break;
    }
    cout << "handler done..." << endl;
}


int main()
{
    signal(SIGCHLD, handler);
    int i = 1;
    for (;i <= 10;i++)
    {
        id = fork();
        if (id == 0)
        {
            int cnt = 5;
            while (cnt--)
            {
                cout << "i am child " << getpid() << " " << getppid() << endl;
                sleep(1);
            }
            exit(2);
        }
    }
    while (1);
}








// void handler(int signo)
// {
//     cout << "quit has changed from 0 to 1" << endl;
//     quit = 1;
//     cout << "quit: " << quit << endl;
// }

// int main()
// {
//     signal(2, handler);

//     while (!quit);

//     cout << "mian quit normal!" << endl;
//     return 0;
// }