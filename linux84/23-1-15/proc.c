#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


//int main()
//{
//  printf("aaaaaaaaaaaaaaaaaaaaaaa\n");
//  fork();
//  printf("bbbbbbbbbbbbbbbbbbbbbbb,pid:%d ppid:%d\n",getpid(),getppid());
//  sleep(3);
//  printf("ccccccccccccccccccccccc,pid:%d ppid:%d\n",getpid(),getppid());
//}
//


int main()
{
  int ret=fork();
  if(ret==0)
  {
    while(1)
    {
      printf("我是子进程，pid:%d,ppid:%d,ret:%d,&ret:%p\n",getpid(),getppid(),ret,&ret);
      sleep(10);
    }
  }
  else if(ret>0)
  {
    while(1)
    {
      printf("我是父进程，pid:%d,ppid:%d,ret:%d,&ret:%p\n",getpid(),getppid(),ret,&ret);
      sleep(5);
    }
  }
  return 0;
}
