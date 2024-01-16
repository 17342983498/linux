#include <stdio.h>
#include <string.h>
int main()
{
  //FILE* fp=fopen("bite.txt","w");
  FILE* fp=fopen("bite.txt","r");
  if(!fp)
  {
    perror("fopen()");
    return 1;
  }
  //const char* msg="linux so easy!\n";
  //fwrite(msg,strlen(msg),1,fp);
  char buf[128]={0};
  fread(buf,1,sizeof(buf),fp);
  printf("%s",buf);
  fclose(fp);
  return 0;
}
