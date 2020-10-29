#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
    int ret=open("./linux",O_RDWR|O_CREAT,0664);
    if(ret<0)
    {
        perror("error!\n");
        return -1;
    }
    printf("file oper ret=%d\n",ret);
    while(1)
    {
        sleep(1);
    }
    return 0;
}
