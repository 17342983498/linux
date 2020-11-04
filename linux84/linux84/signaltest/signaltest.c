#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void sigcb(int signo)
{
    printf("signo is %d\n",signo);
}
int main()
{
    signal(2,sigcb);
    signal(20,sigcb);
    signal(9,sigcb);
    while(1)
    {
        sleep(1);
    }
    return 0;
}
