#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


//int main()
//{
//  //FILE* fp = fopen("log.txt","w");
//  FILE* fp = fopen("log.txt","r");
//  if(!fp)
//  {
//    perror("fopen\n");
//    return 1;
//  }
//  //const char* msg = "i love you\n";
//  //fwrite(msg,strlen(msg),1,fp);
//  char buf[128]={0};
//  fread(buf,sizeof(buf),1,fp);
//  printf("%s",buf);
//  fclose(fp);
//  return 0;
//}
//


//int main()
//{
//  //int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC);
//  int fd = open("log.txt",O_RDONLY);
//  if(fd<0)
//  {
//    perror("open");
//    return 1;
//  }
//  //int count=2;
//  const char* msg="bbbbbbbbbbbbb\n";
//  //while(count--)
//  //{
//  //  write(fd,msg,strlen(msg));
//  //}
//  char buf[128]={0};
//  read(fd,buf,strlen(msg));
//  printf("%s",buf);
//  close(fd);
//  return 0;
//}


//int main()
//{
//  char buf[128]={0};
//  read(0,buf,sizeof(buf));
//  printf("%s",buf);
//  write(1,buf,strlen(buf));
//  write(2,buf,strlen(buf));
//  return 0;
//}
//

int main()
{
  int fd=open("log.txt",O_WRONLY|O_CREAT|O_TRUNC);
  //int fd=open("log.txt",O_RDONLY);
  if(fd<0)
  {
    perror("open");
    return 1;
  }
  //const char* msg="aaaaaabbbbbbb\n";
  //write(1,msg,strlen(msg));
  char buf[128]={0};
  //read(0,buf,sizeof(buf));
  //printf("%s\n",buf);
  //
  close(1);
  dup2(fd,1);
  read(0,buf,sizeof(buf));
  printf("%s",buf);
  fflush(stdout);
  close(fd);
  return 0;
}
