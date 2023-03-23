#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
//int main()
//{
//  int pid = fork();
//  if(pid<0)
//  {
//    perror("fork fail!");
//    exit(1);
//  }
//  else if(pid==0)
//  {
//    //子进程
//    printf("i am child,pid=%d\n",getpid());
//    sleep(5);
//    exit(107);
//  }
//  else{
//    int status=0;
//    int ret=wait(&status);
//    if(ret==-1)
//    {
//      printf("wait fail!\n");
//    }
//    else
//    {
//      printf("wait success,child exit code:%d,child exit status:%d\n",(status>>8)&0xff,(status&0x7f));
//    }
//  }
//
//  return 0;
//}


int main()
{
  int pid = fork();
  if(pid<0)
  {
    perror("fork fail\n");
    exit(1);
  }
  else if(pid==0)
  {
    int cnt=5;
    while(cnt--)
    {
    printf("i am child, i also running,pid:%d\n",getpid());
    sleep(1);

    }
    exit(107);
  }
  else{
    while(1)
    {
      int status =0;
      //int ret = waitpid(-1,&status,0);
      int ret = waitpid(pid,&status,WNOHANG);
      if(WIFEXITED(status)&&ret==pid)
      {
        printf("wait child success, child return code is:%d,child return signal:%d\n",WEXITSTATUS(status),status&0x7f);
        break;
      }
      else if(ret==0)
      {
        printf("子进程还未退出，我再做做其他事\n");
        sleep(1);
        continue;
      }
      else{
        printf("wait child fail\n");
        exit(1);
      }
    }
  }
  return 0;
}

