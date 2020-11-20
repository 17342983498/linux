#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    printf("-------begin-------");
    //fflush();
    //_exit(0);
    exit(1);
    printf("--------end--------\n");
    return 0;
}
