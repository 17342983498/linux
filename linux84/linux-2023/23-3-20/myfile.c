#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


//int main()
//{
//  int fd = open("log.txt",O_RDONLY);
//  //int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0664);
//  if(fd<0)
//  {
//    perror("open");
//    return 1;
//  }
//
//  const char* msg="aaaaaaaaaaa";
//  char buf[127]={0};
//  //read(fd,buf,strlen(msg));
//  //printf("%s\n",buf);
//  read(0,buf,strlen(msg));
//  printf("%s",buf);
//
//  //const char* msg="aaaaaaaaaaa\n";
//  //write(fd,msg,strlen(msg));
//  //const char* buf="hello world!\n";
//  //write(fd,buf,strlen(buf));
//  //write(fd,buf,strlen(buf));
//  //write(fd,buf,strlen(buf));
//  //write(fd,buf,strlen(buf));
//  //write(fd,buf,strlen(buf));
//  close(fd);
//  return 0;
//}
//
//


//int main()
//{
//  char buf[128]={0};
//  ssize_t s=read(0,buf,sizeof(buf));
//  if(s>0)
//  {
//    buf[s]=0;
//    write(1,buf,strlen(buf));
//    write(2,buf,strlen(buf));
//  }
//  return 0;
//}



//int main()
//{
//  close(1);
//  open("file_normal.txt",O_WRONLY|O_CREAT,0664);
//  close(2);
//  open("file_error.txt",O_WRONLY|O_CREAT,0664);
//
//  printf("hello printf->stdout\n");
//  printf("hello printf->stdout\n");
//  printf("hello printf->stdout\n");
//  printf("hello printf->stdout\n");
//  fprintf(stdout, "hello fprintf->stdout\n");
//  fprintf(stdout, "hello fprintf->stdout\n");
//  fprintf(stdout, "hello fprintf->stdout\n");
//  fprintf(stdout, "hello fprintf->stdout\n");
//  fprintf(stdout, "hello fprintf->stdout\n");
//  fprintf(stderr, "hello fprintf->stderr\n");
//  fprintf(stderr, "hello fprintf->stderr\n");
//  fprintf(stderr, "hello fprintf->stderr\n");
//  fprintf(stderr, "hello fprintf->stderr\n");
// 
//  
//  
//  
//  
//  //const char* msg="sdfsfs";
//  //write(fd,msg,strlen(msg));
//  //printf("fd:%d\n",fd);
//  
// // const char* msg="aaaaaaaaaaaaaaaaa\n";
// // printf("%s",msg);
// // printf("bbbbbbbbbbbbbbbb\n");
// // //printf("%d\n",fd);
// // fflush(stdout);
//  return 0;
//}
//

int main()
{
    //C库
    fprintf(stdout, "hello fprintf\n");
    printf("hello printf\n");
    const char* msg="hello fwrite\n";
    fwrite(msg,strlen(msg),1,stdout);
    //系统调用
    const char *msg1 = "hello write\n";
    write(1, msg1, strlen(msg1)); //+1?

    fork(); //????
    return 0;
}




