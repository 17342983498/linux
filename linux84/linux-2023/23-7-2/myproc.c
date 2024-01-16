#include <stdio.h>
#include <unistd.h>


//int main(int argc,char *argv[],char *envp[])
//{
//  //int cnt=5;
//  //while(cnt--)
//  //{
//  //  printf("hello %d\n",cnt);
//  //  sleep(1);
//  //}
//  //
//  int i=0;
//  for(;envp[i];++i)
//  {
//    printf("envp[%d]:%s\n",i,envp[i]);
//  }
//  return 0;
//}


//int main()
//{
//  extern char **environ;
//  int i=0;
//  for(;environ[i];++i)
//  {
//    printf("environ[%d]:%s\n",i,environ[i]);
//  }
//  return 0;
//}
//

#include <stdlib.h>

int main()
{
  char *path=getenv("PATH");
  if(path==NULL)
    perror("getenv");
  else printf("PATH:%s\n",path);
  return 0;
}
