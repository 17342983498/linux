#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int Func(int num)
{
  int sum=0;
  for(int i=1;i<=num;i++)
  {
    sum+=i;
  }
  return sum;
}

#define TASK_NUM 10

void sync_disk()
{
    printf("这是一个刷新数据的任务!\n");
}

void sync_log()
{
    printf("这是一个同步日志的任务!\n");
}

void net_send()
{
    printf("这是一个进行网络发送的任务!\n");
}



typedef void (*func_t)();
func_t other_task[TASK_NUM] = {NULL};  //函数指针数组

int LoadTask(func_t func)
{
    int i = 0;
    for(; i < TASK_NUM; i++){
        if(other_task[i] == NULL) break;
    }
    if(i == TASK_NUM) return -1;
    else other_task[i] = func;

    return 0;
}

void InitTask()
{
    for(int i = 0; i < TASK_NUM; i++) other_task[i] = NULL;
    LoadTask(sync_disk);
    LoadTask(sync_log);
    LoadTask(net_send);
}

void RunTask()
{
    for(int i = 0; i < TASK_NUM; i++)
    {
        if(other_task[i] == NULL) continue;
        other_task[i]();
    }
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

    _exit(123);
  }
  InitTask();
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
      //printf("子进程还未退出，我干干其他事！\n");
      RunTask();
      sleep(1);
    }
    else
    {
      if(WIFEXITED(status))
      {
        printf("我是父进程，等待子进程成功,pid:%d,ppid:%d,status signal:%d,status code:%d\n",getpid(),getppid(),(status&0x7F),WEXITSTATUS(status));
      }
      else
        printf("我是父进程，等待子进程成功,pid:%d,ppid:%d,status signal:%d,status code:%d\n",getpid(),getppid(),(status&0x7F),((status>>8)&0xff));
      //printf("我是父进程，等待子进程成功,pid:%d,ppid:%d,status signal:%d,status code:%d\n",getpid(),getppid(),(status&0x7F),((status>>8)&0xff));
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
