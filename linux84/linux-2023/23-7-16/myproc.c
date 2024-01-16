#include <stdio.h>
#include <unistd.h>
#include <assert.h>
// int main()
// {
//   while(1)
//   {
//     printf("hello world!\n");
//     sleep(1);
//   }
//   return 0;
// }

int num=10;

int main()
{
  int id=fork();
  assert(id>=0);
  if(id==0)
  {
    //子进程
    while(1)
    {
      printf("i am child,num:%d,&num:%p\n",num,&num);
      sleep(1);
    }
  }
  else
  {
    int cnt=5;
    //父进程
    while(1)
    {
      printf("i am father,num:%d,&num:%p\n",num,&num);
      if(--cnt==0)
      {
        num=20;
      }
      sleep(1);
    }
  }
  return 0;
}
