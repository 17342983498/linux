#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
    int fd = open("./fifo",O_RDWR);
    if(fd < 0)
    {
        perror("fifo()");
        return -1;
    }
    char buf[1024]={0};
    read(fd,buf,sizeof(buf)-1);
    printf("buf : %s\n",buf);
    while(1)
    {
        sleep(1);
    }
    close(fd);
    return 0;
}
