#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
  printf("1111111111111111111111111 pid:%d, ppid:%d\n",getpid(),getppid());
  pid_t ret = fork();
  printf("2222222222222222222222222 pid:%d, ppid:%d,ret:%d,&ret:%p\n",getpid(),getppid(),ret,&ret);


    //while(1) 
    //{ 
    //    printf("process running!, pid:%d, ppid:%d\n",getpid(),getppid());
    //    sleep(1);
    //}
    return 0;
}
