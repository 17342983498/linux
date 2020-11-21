#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void func()
{
    printf("-----end-----\n");
}
int main()
{
    atexit(func);
    printf("-----begin-----\n");
    return 0;
}
