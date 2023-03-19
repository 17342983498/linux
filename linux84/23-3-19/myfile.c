#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>



//int main()
//{
//  FILE* fp=fopen("LOG.txt","w");
//  if(!fp)
//  {
//    perror("fopen fail!\n");
//    return -1;
//  }
//  const char* msg = "hello world!";       
//  int cnt = 5;
//  while(cnt--)
//  {
//     fputs(msg,fp);                                                                                                                     
//     printf("%s\n",msg);
//  }
//  fclose(fp);
//  return 0;
//}


//int main() 
//{
//  FILE* pf=fopen("LOG.txt","r");
//  if(!pf)
//  {
//    perror("fopen\n"); 
//    return -1;
//  }
//  while(1)
//  {
//    char buf[128]={0};
//    if(fgets(buf,sizeof(buf),pf)==NULL) 
//    {
//      break;
//    }
//    else{
//      printf("%s\n",buf);
//    }
//  }
//  fclose(pf);
//  return 0;
//}
//
//int main()
//{
//  FILE* pf=fopen("LOG.txt","w");
//  const char* msg="BBBBB";
//  int cnt=1;
//  while(cnt)
//  {
//    fputs(msg,pf);
//    //break;
//    char buf[256]={0};
//    snprintf(buf,sizeof(buf),"%s:%d\n",msg,cnt);
//    fputs(buf,pf);
//    printf("%s",buf);
//    //fgets(buf,sizeof(buf),pf);
//    //printf("------------------\n");
//    //printf("%s",buf);
//    //printf("-----------------\n");
//    //printf("-----------------\n");
//    fprintf(stdout,"%s:%d\n",msg,cnt);
//    fprintf(pf,"%s:%d\n",msg,cnt);
//    //fgets(buf,sizeof(buf),pf);
//    printf("%s",buf);
//    cnt--;
//  }
//  return 0;
//}
//
//



//int main()
//{
//  umask(0);
//  int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0664);
//  if(fd<0)
//  {
//    perror("open");
//    return -1;
//  }
//  int count=5;
//  const char* msg="hello world!";
//  while(count--)
//  {
//    write(fd,msg,strlen(msg));
//  }
//  close(fd);
//  return 0;
//}
//


//int main()
//{
//  int fd=open("log.txt",O_RDONLY);
//  if(fd<0)
//  {
//    perror("open()");
//    return -1;
//  }
//  char buf[128]={0};
//  ssize_t s = read(fd,buf,sizeof(buf)-1);
//  if(s>0)
//  {
//    printf("%s\n",buf);
//  }
//  close(fd);
//  return 0;
//}
//
//
//



int main()
{
  umask(0);
  int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0664);
  if(fd<0)
  {
    perror("open");
    return -1;
  }
  int count=5;
  const char* msg="=====\n";
  while(count--)
  {
    write(fd,msg,strlen(msg));
  }
  close(fd);
  return 0;
}
