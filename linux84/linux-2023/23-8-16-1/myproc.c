#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// int main()
// {
//     printf("-----------------\n");
//     printf("-----------------\n");
//     execl("/bin/ls","ls","-a","-l",NULL);
//     printf("-----------------\n");
//     printf("-----------------\n");
// }


int main()
{
    extern **environ;
    int pid=fork();
    if(pid==0)
    {
        // printf("i am child,pid:%d\n",getpid());
        / execl("/bin/ls","ls","-a","-l",NULL);
        char *const MYENV[]={"MYENV=YouCanSeeMe",NULL};
        // execle("./exec/test","test",NULL,MYENV);
        putenv("MYENV=YouCanSeeMe");
        execle("./exec/test","test",NULL,environ);
    }

    //父进程
    sleep(5);
    int status=0;
    printf("i am father,pid:%d\n",getpid());
    waitpid(pid,&status,0);
}
