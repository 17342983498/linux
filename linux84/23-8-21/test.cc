#include <iostream>
#include <cstdio>
using namespace std;

int main()
{
  //C
  printf("printf->stdout\n");
  fprintf(stdout,"fprintf->stdout\n");
  fprintf(stderr,"fprintf->stderr\n");

  //C++
  cout<<"cout->cout"<<endl;
  cerr<<"cerr->cerr"<<endl;
  return 0;
}
