#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>


int main()
{
    int fd = open ("./fifo",O_RDWR);
    if(fd < 0)
    {
        perror("open()");
        return -1;
    }
    write(fd,"linux is 666",12);
    while(1)
    {
        sleep(1);
    }
    close(fd);
    return 0;
}
