#include <stdio.h>
#include <unistd.h>

int main()
{
  pid_t id=fork();
  if(id==0)
  {
    //子进程
    while(1)
    {
      printf("i am child,pid:%d,ppid:%d\n",getpid(),getppid());
      sleep(1);
    }
  }
  else if(id>0)
  {
    //父进程 
    int cnt=10;
    while(1)
    {
      printf("i am father,pid:%d,ppid:%d\n",getpid(),getppid());
      if(cnt-- <= 0)
      {
        break;
      }
      sleep(1);
    }
  }
  return 0;
}
