#include<stdio.h>
#include<unistd.h>
int main()
{
    int ret=fork();
    if(ret<0)
    {
        perror("fork1()\n");
    }
    else if(ret==0)
    {
        putchar('b');
    }
    else
    {
        int ert=fork();
        if(ert<0)
        {
            perror("fork2()\n");
        }
        else if(ert==0)
        {
            putchar('c');
        }
        else
        {
            putchar('a');
        }
    }
    return 0;
}
