#include "mystdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

MY_FILE *my_open(const char *path, const char *mode)
{
    // 1.先确定打开文件的方式
    int flag=0;
    if(strcmp(mode,"r") == 0)
    {
        flag |= O_RDONLY;
    }
    else if(strcmp(mode,"w") == 0)
    {
        flag |= O_WRONLY | O_CREAT | O_TRUNC;
    }
    else if(strcmp(mode,"a") == 0)
    {
        flag |= O_WRONLY | O_CREAT | O_APPEND;
    }

    // 2.打开文件 
    mode_t m = 0666;
    int  fd = 0;
    if(flag & O_CREAT)
    {
        fd = open(path, flag, m);
    }
    else 
    {
        fd = open(path, flag);
    }

    if(fd < 0) return NULL;
    
    // 3. 构建MY_FILE对象
    MY_FILE* mf = (MY_FILE*)malloc(sizeof(MY_FILE));
    if(mf == NULL)
    {
        close(fd);
        return NULL;
    }

    //初始化对象
    mf->fd = fd;
    mf->flags = BUFF_LINE;
    memset(mf->buffer, '\0',sizeof(mf->buffer));
    mf->current = 0;
}

size_t my_write(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream)
{
    // 1. 如果缓冲区已满，则先刷新
    if(stream->current == NUM)
    {
        my_fflush(stream);
    }

    // 2. 根据缓冲区剩余情况进行数据拷贝
    int writen = 0;
    size_t user_size = size * nmemb;
    size_t my_size = NUM - stream->current;

    if(my_size >= user_size)
    {
        memcpy(stream->buffer + stream->current, ptr, user_size);

        // 3. 更新计数器字段
        stream->current += user_size;
        writen = user_size;
    }
    else
    {
        memcpy(stream->buffer + stream->current, ptr, my_size);

        // 3. 更新计数器字段
        stream->current += my_size;
        writen = my_size;
    }

    // 4. 刷新
    if(stream->flags & BUFF_ALL)
    {
        if(stream->current == NUM) my_fflush(stream);
    }
    else if(stream->flags & BUFF_LINE)
    {
        if(stream->buffer[stream->current-1] == '\n') my_fflush(stream);
    }
    else
    {
        //TODO
    }
    return writen;
    return 0;
}

int my_fflush(MY_FILE *fp)
{
    assert(fp);
    //将用户缓冲区中的数据，通过系统调用接口，冲刷给OS
    write(fp->fd, fp->buffer, fp->current);
    fp->current = 0;

    //fsync(fp->fd);
    return 0;
}

int my_close(MY_FILE *fp)
{
    assert(fp);
    // 1. 冲刷缓冲区
    if(fp->current > 0)
    {
        my_fflush(fp);
    }
    // 2.关闭文件
    close(fp->fd);
    // 3. 释放堆空间
    free(fp);
    fp = NULL;

    return 0;
}