#include <stdio.h>
#include <unistd.h>

int main()
{
    int ret = fork();
    if(ret<0)
    {
        perror("fork()\n");
        return -1;
    }
    else if(ret==0)
    {
        while(1)
        {
            printf("i am child, pid=%d,ppid=%d\n",getpid(),getppid());
            sleep(1);
        }
    }
    else
    {
        while(1)
        {
            printf("i am father, pid=%d,ppid=%d\n",getpid(),getppid());
            sleep(1);
        }
    }
    return 0;
}
