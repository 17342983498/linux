#include "myadd.h"
#include "mysub.h"
#include <stdio.h>
int main()
{
  int x=100,y=50;
  printf("%d+%d=%d\n",x,y,myadd(x,y));
  printf("%d-%d=%d\n",x,y,mysub(x,y));
  return 0;
}
