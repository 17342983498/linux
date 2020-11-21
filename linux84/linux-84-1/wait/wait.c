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
        int count=10;
        while(count--)
        {
            printf("i am child! pid = [%d],ppid = [%d]\n",getpid(),getppid());
            sleep(1);
        }
    }
    else
    {
        while(1)
        {
            //int status;
            //wait(&status);
            //if((s>0)&&(status&0x7F)==0)
            //{
            //    printf("child exit code:%d\n",(status>>8)&0xFF);
            //}
            //else if(s>0)
            //{
            //    printf("sig code : %d\n",status&0x7F);
            //}
            printf("i am father! pid = [%d],ppid = [%d]\n",getpid(),getppid());
            sleep(1);
        }
    }
}
