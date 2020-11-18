#include <stdio.h>
#include <unistd.h>

int g_val = 10;

int main()
{
    int ret = fork();
    if(ret < 0)
    {
        perror("fork()\n");
        return -1;
    }
    else if(ret == 0)
    {
        while(1)
        {
            printf("i am child,%d,%p\n",g_val,&g_val);
            sleep(1);
        }
    }
    else
    {
        while(1)
        {
            g_val = 20;
            printf("i am father,%d,%p\n",g_val,&g_val);
            sleep(1);
        }
    }
    return 0;
}
