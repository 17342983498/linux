#include "mystdio.h"


int main()
{
    MY_FILE* mf = my_open("log.txt", "w");
    if(mf == NULL)
    {
        return 1;
    }

    const char *str = "hello my fwrite";
    int cnt = 5;
    //操作文件
    while(cnt)
    {
        char buffer[1024];
        //snprintf(buffer, sizeof(buffer), "%s:%d", str, cnt--);
        snprintf(buffer, sizeof(buffer), "%s:%d\n", str, cnt--);
        size_t size = my_write(buffer, strlen(buffer), 1, mf);
        sleep(1);
        printf("当前成功写入: %lu个字节\n", size);
        //my_fflush(mf);

        //if(cnt % 5 == 0)
            //my_fwrite("\n", strlen("\n"), 1, mf);

   }

    my_close(mf);
    return 0;
}