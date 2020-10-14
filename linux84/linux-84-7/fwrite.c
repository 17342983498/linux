#include <stdio.h>
#include <string.h>
int main()
{
    FILE* pf = fopen("./linux84","w+");
    if(!pf)
    {
        perror("fopen()");
        printf("open file failed\n");
        return -1;
    }
    printf("open file success!\n");
    const char* arr = "linux so easy!";
    ssize_t ret = fwrite(arr,1,strlen(arr),pf);
    printf("write block count is %lu\n",ret);
    fseek(pf,-7,SEEK_END);
    char buf[1024]={0};
    ret = fread(buf,1,4,pf);
    printf("ret=%lu\n",ret);
    printf("buf:%s\n",buf);
    fclose(pf);
    return 0;
}
