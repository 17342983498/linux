#include<stdio.h>
#include<unistd.h>

int main()
{
    printf("哈哈---------！\n");
    pid_t ret=fork();
    if(ret<0)
    {
        perror("fork");
        return 0;
    }
    else if(ret>0)
    {
        while(1)
        {
            printf("i am father,pid=[%d],ppid=[%d]\n",getpid(),getppid());
            sleep(1);
        }
    }
    else
    {
        while(1)
        {
            printf("i am child,pid=[%d],ppid=[%d]\n",getpid(),getppid());
            sleep(1);
        }
    }
    return 0;
}
