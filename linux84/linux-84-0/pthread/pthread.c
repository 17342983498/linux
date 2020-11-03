#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void* start_thread(void* arg)
{
    while(1)
    {
        printf("linux 666\n");
        sleep(1);
    }
}
int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,start_thread,NULL);
    if(ret < 0)
    {
        perror("pthread_create\n");
        return -1;
    }
    while(1)
    {
        printf("i am main thread\n");
        sleep(1);
    }
    return 0;
}
