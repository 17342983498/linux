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
  pid_t id=fork();
  if(id==0)
  {
    //子进程
    int cnt=5;
    while(cnt--)
    {
      printf("我是子进程，我现在活着呢，我离死亡还有%d秒,pid:%d,ppid:%d\n",cnt,getpid(),getppid());
      sleep(1);
    }
    // int a=10;
    // a/=0;
    _exit(123);
  }
  //sleep(5);
  while(1)
  {
    int status=0;
    pid_t ret_id=waitpid(id,&status,WNOHANG);
    if(ret_id==-1)
    {
      printf("等待错误!\n");
      break;
    }
    else if(ret_id==0)
    {
      printf("子进程还未退出，我干干其他事！\n");
      sleep(1);
    }
    else
    {
      printf("我是父进程，等待子进程成功,pid:%d,ppid:%d,status signal:%d,status code:%d\n",getpid(),getppid(),(status&0x7F),((status>>8)&0xff));
      break;
    }
  }
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
