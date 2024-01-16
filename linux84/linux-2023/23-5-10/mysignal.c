#include <stdio.h>
#include <signal.h>

volatile int quit=0;

void handler(int signo)
{
    printf("change quit 0 to 1 \n");
    quit=1;
    printf("quit: %d\n",quit);
}

int main()
{
    signal(2,handler);
    while(!quit);
    printf("man quit 正常！\n");

    return 0;
}