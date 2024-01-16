#include <stdio.h>


#define NUM 100
#define BUFF_NONE 0x1
#define BUFF_LINE 0x2
#define BUFF_ALL  0x4

typedef struct MY_FILE
{
    int fd;
    char buffer[NUM];
    int flags;
    int current;
}MY_FILE;


MY_FILE *my_open(const char *path, const char *mode);
size_t my_write(const void *ptr, size_t size, size_t nmemb,MY_FILE *stream);
int my_fflush(MY_FILE *fp);
int my_close(MY_FILE *fp);
