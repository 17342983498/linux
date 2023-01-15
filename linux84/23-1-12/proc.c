#include "proc.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void process()
{
  char bar[102];
  memset(bar,0,sizeof(bar));
  int i=0;
  const char* lable="|/-\\";
  while(i <= 100)
  {
    printf("[%-100s][%d%%][%c]\r",bar,i,lable[i%4]);
    fflush(stdout);
    bar[i++]='=';
    if(i!=100)
      bar[i]='>';
    usleep(100000);
  }
  printf("\n");
  printf("加载完成！\n");
}
