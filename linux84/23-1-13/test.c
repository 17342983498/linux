#include <stdio.h>

int add(int num)
{
  int sum=0;
  for(int i=1;i<=num;i++)
  {
    sum+=i;
  }
  return sum;
}


int main()
{
  int num=50;
  int sum = add(num);
  printf("%d\n",sum);
  return 0;
}
