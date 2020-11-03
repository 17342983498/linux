#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


struct pthreadData
{
    int _num;
    pthreadData()
    {
        _num = -1;
    }
};
void* start_thread(void* arg)
{
    pthreadData* td = (pthreadData*) arg;
    //while(1)
    {
        printf("i am work thread %d\n",td->_num);
        sleep(1);
    }
    pthread_exit(NULL);
   // while(1)
   // {
    //    sleep(1);
   // }
    pthread_cancel(pthread_self());
    delete td;
}
int main()
{
    pthread_t tid;
    int i = 0;
    for(i=0;i<4;i++)
    {
        struct pthreadData* td = new pthreadData;
        td->_num = i;
        int ret = pthread_create(&tid,NULL,start_thread,(void*)td);
        if(ret < 0)
        {
            perror("pthread_create\n");
            return -1;
        }
    }
    //pthread_exit(NULL);
    while(1)
    {
        printf("i am main thread\n");
        sleep(1);
    }
    return 0;
}
