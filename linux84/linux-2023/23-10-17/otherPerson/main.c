#include <stdio.h>
#include "myadd.h"
#include "mysub.h"


int main()
{
  int x = 200,y = 100;
  printf("%d + %d = %d\n",x,y,myadd(x,y));
  printf("%d - %d = %d\n",x,y,mysub(x,y));

  return 0;
}
