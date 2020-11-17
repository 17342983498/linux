#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int a = 10;
    int b = 20;
    int c = a + b;
    while(1)
    {
        printf("c:%d\n",c);
        sleep(1);
    }
    return 0;
}
