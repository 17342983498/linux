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
    while(1)
    {
      printf("i am father,pid:%d,ppid:%d\n",getpid(),getppid());
      sleep(1);
    }
  }
  return 0;
  //int count=0;
  //while(1)
  //{  
  //  printf("我再运行吗??? count:%d\n",count++);
  //  sleep(1);
  //}
  //return 0;
}
