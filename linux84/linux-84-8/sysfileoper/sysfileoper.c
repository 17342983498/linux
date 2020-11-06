#include<stdio.h>
#include<fcntl.h>

int main()
{
    int fd = open("./linux84",O_WRONLY|O_CREAT,0664);
    if(fd<0)
    {
        perror("open");
        return -1;
    }
    const char* arr="linux is so easy!";
    
    return 0;
}
