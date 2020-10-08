#include<stdio.h>
#include<unistd.h>
int main()
{
    int fd[2];
    int ret=pipe(fd);
    if(ret<0)
    {
        perror("pipe");
        return 0;
    }
    printf("fd[0]:%d\n",fd[0]);
    printf("fd[1]:%d\n",fd[1]);
    ret=fork();
    if(ret<0)
    {
        perror("fork!");
        return 0;
    }
    else if(ret==0)
    {
        close(fd[0]);
        write(fd[1],"linux is 666!",13);
    }
    else
    {
        close(fd[1]);
        sleep(5);
        char buf[1024]={0};
        read(fd[0],buf,sizeof(buf)-1);
        printf("i am father:buf if %s\n",buf);
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}
