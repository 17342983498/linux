#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main()
{
    int ret = fork();
    if(ret<0)
    {
        perror("fork\n");
        return -1;
    }
    else if(ret == 0)
    {
        //int count=10;
        while(1)
        {
            int *p = NULL;
            *p = 1;
            printf("i am child! pid = [%d],ppid = [%d]\n",getpid(),getppid());
            sleep(1);
        }
        exit(10);
    }
    else
    {
        int status;
        wait(&status);
        while(1)
        {
            printf("sig code : %d\n",status&0x7F);
            printf("coredump code : %d\n",(status>>7)&0x1);
            //printf("exit code %d\n",(status>>8)&0xFF);
            printf("i am father! pid = [%d],ppid = [%d]\n",getpid(),getppid());
            sleep(1);
        }
    }
}
