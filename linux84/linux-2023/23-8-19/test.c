#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <error.h>
#include <errno.h>



int main()
{
    umask(0);
    // int fd = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
    int fd = open("log.txt",O_RDONLY);
    if(fd == -1)
    {
        printf("fd:%d,errno:%d,errstring:%s\n",fd,errno,strerror(errno));
    }
    else
    {
        printf("fd:%d,errno:%d,errstring:%s\n",fd,errno,strerror(errno));
    }
    char buffer[1024];
    ssize_t n = read(fd,buffer,sizeof(buffer)-1);
    if(n > 0)
    {
        buffer[n]='\0';
        printf("%s",buffer);
    }

    // const char *msg="asdassasdas";
    // char buffer[128];
    // snprintf(buffer,sizeof(buffer),"%s:%d\n",msg,fd);
    // write(fd, buffer, strlen(buffer));
    close(fd);
    return 0;
}






// int main()
// {
//     FILE* fp = fopen("log.txt","a");
//     if(fp == NULL)
//     {
//         perror("fopen");
//         return 1;
//     }
//     const char *msg="sadassa\n";
//     fputs(msg,fp);
//     // fputs(msg,fp);
//     // int cnt=5;
//     // while(cnt--)
//     // {
//     //     //fprintf(fp,"%s:%d\n",msg,cnt);
//     //     char buffer[256]={0};
//     //     snprintf(buffer,sizeof(buffer),"%s:%d\n",msg,cnt);
//     //     fputs(buffer,fp);
//     // }
//     fclose(fp);
//     return 0;
// }