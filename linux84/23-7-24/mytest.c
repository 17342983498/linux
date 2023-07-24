#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int Func(int num)
{
  int sum=0;
  for(int i=1;i<=num;i++)
  {
    sum+=i;
  }
  return sum;
}



int main()
{
  pid_t ret=fork();
  if(ret==0)
  {
    //子进程
    int cnt=2;
    while(cnt--)
    {
      printf("我是子进程，我现在活着呢，我离死亡还有%d秒,pid:%d,ppid:%d\n",cnt,getpid(),getppid());
      sleep(1);
    }
    _exit(0);
  }
  sleep(5);
  pid_t ret_id=wait(NULL);
  printf("我是父进程，等待子进程成功,pid:%d,ppid:%d\n",getpid(),getppid());
  return 0;
}

//int main() 
//{ 
//  for(int i=0;i<200;i++)
//  {
//    printf("%d:%s\n",i,strerror(i));
//  }
//  return 0;
//
// // int ret=Func(100);
// // if(ret==5050)
// //   return 1;
// // else 
// //   return 0;
//}
