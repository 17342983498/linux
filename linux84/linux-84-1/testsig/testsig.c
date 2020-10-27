#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main()
{
    printf("open successed!\n");
    //kill(getpid(),2); 
    raise(2);
    printf("hahaha\n");
    while(1)
    {
        sleep(1);
    }
    return 0;
}
