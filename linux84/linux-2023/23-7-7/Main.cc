#include "RingQueue.hpp"
#include <ctime>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include "Task.hpp"

using namespace std;

const char *ops="+-*/%";

void *consumer(void *args)
{
    RingQueue<Task> *rq = static_cast<RingQueue<Task> *>(args);
    while(true)
    {
        Task t;
        rq->pop(&t); //任务拿出来
        t(); //处理任务--计算
        cout << "consumer done, 处理完成的任务是：" << t.formatRes() << endl;

        // sleep(1);
        // int data=0;
        // rq->pop(data);
        // cout<<"consumer done : "<< data <<endl;
    }

}

void *productor(void *args)
{
    RingQueue<Task> *rq = static_cast<RingQueue<Task> *>(args);
    while(true)
    {
        int x = rand() % 100;
        int y = rand() % 100;
        char op = ops[(x + y) % strlen(ops)];
        Task t(x,y,op);
        rq->push(t);
        cout << "productor done, 生产的任务是: " << t.formatArg() << endl;

        // int data = rand() % 10 + 1;
        // rq->push(data);
        // cout<<"productor done : "<< data <<endl;
    }
}


int main()
{
    srand(time(nullptr) ^ getpid());
    RingQueue<Task> *rq = new RingQueue<Task>();

    pthread_t c[3],p[2];
    for(int i=0;i<3;++i)
        pthread_create(c+i,nullptr,consumer,(void*)rq);
    for(int i=0;i<2;++i)
        pthread_create(p+i,nullptr,productor,(void*)rq);
    for(int i=0;i<3;++i)
        pthread_join(c[i],nullptr);
    for(int i=0;i<2;++i)
        pthread_join(p[i],nullptr);

    delete rq;
    return 0;
}



