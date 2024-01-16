#include <pthread.h>
#include <stdio.h>


void *thread_run1(void *args)
{
    while(1)
    {
        printf("我是线程1，我正在执行我的代码！\n");
        sleep(1);
    }
}

void *thread_run2(void *args)
{
    while(1)
    {
        printf("我是线程2，我正在执行我的代码！\n");
        sleep(1);
    }
}

void *thread_run3(void *args)
{
    while(1)
    {
        printf("我是线程3，我正在执行我的代码！\n");
        sleep(1);
    }
}

int main()
{
    pthread_t t1,t2,t3;
    pthread_create(&t1,NULL,thread_run1,NULL);
    pthread_create(&t2,NULL,thread_run2,NULL);
    pthread_create(&t3,NULL,thread_run3,NULL);

    while(1)
    {
        printf("我是主线程，我正在执行我的代码！\n");
        sleep(1);
    }
}