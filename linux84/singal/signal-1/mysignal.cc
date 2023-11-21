#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;

int count = 0;

void handler(int signo)
{
    cout << "get a signal: " << signo << " " << count << endl;
    // int n = alarm(10);
    // cout << "return: " << n << endl;
    exit(1);
}

int main()
{
    signal(SIGALRM, handler);
    //cout << getpid() << endl;
    alarm(1);

    while (1)
    {
        count++;
    }
}








// int main()
// {
//     while (1)
//     {
//         cout << "我是一个进程，我正在运行！！！我的pid：" << getpid() << endl;
//         sleep(1);
//     }
// }



// void handler(int signo)
// {
//     cout << "get a signal: " << signo << endl;
// }

// int main()
// {
//     for (int i = 1;i < 32;i++)
//     {
//         signal(i, handler);
//     }
//         while (1)
//     {
//         cout << "我是一个进程，我正在运行！！！我的pid：" << getpid() << endl;
//         sleep(1);
//     }
// }