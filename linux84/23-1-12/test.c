#include <stdio.h>
#include <unistd.h>

int main()
{
  int i;
  for(i=10;i>=0;i--)
  {
    printf("%-2d\r",i);
    fflush(stdout);
    sleep(1);
  }
  return 0;
}
