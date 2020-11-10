#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <semaphore.h>
#include <pthread.h>

#define CAPACITY 1
#define THREADCOUNT 1

class RingQueue
{
public:
    RingQueue()
    :vec_(CAPACITY)
    {
        capacity_= CAPACITY;
        pos_read_ = 0;
        pos_write_ = 0;
        sem_init(&lock_,0,1);
        sem_init(&write_,0,CAPACITY);
        sem_init(&read_,0,0);
    }
    ~RingQueue()
    {
        sem_destroy(&write_);
        sem_destroy(&read_);
    }
    void Push(int Data)
    {
        sem_wait(&write_);
        sem_wait(&lock_);
        vec_[pos_write_] = Data;
        pos_write_=(pos_write_+1) % capacity_;
        sem_post(&lock_);
        sem_post(&read_);
    }
    void Pop(int* Data)
    {
        sem_wait(&read_);
        sem_wait(&lock_);
        *Data=vec_[pos_read_];
        pos_read_ = (pos_read_ + 1) % capacity_;
        sem_post(&lock_);
        sem_post(&write_);
    }
private:
    std::vector<int> vec_;
    int capacity_;
    sem_t lock_;
    sem_t read_;
    sem_t write_;
    int pos_read_;
    int pos_write_;
};

void* readstart(void* arg)
{
    RingQueue* rq = (RingQueue*)arg;
    while(1)
    {
        int data;
        rq->Pop(&data);
        printf("i am %p,i read %d\n",pthread_self(),data);
    }
    return NULL;
}


void* writestart(void* arg)
{
    RingQueue* rq = (RingQueue*)arg;
    int data = 0;
    while(1)
    {
        rq->Push(data);
        printf("i am %p,i write %d\n",pthread_self(),data);
        data++;
    }
    return NULL;
}


int main()
{
    int i =0;
    pthread_t rd[THREADCOUNT],wr[THREADCOUNT];
    RingQueue* rq = new RingQueue();
    for(i = 0;i<THREADCOUNT;i++)
    {
       int ret = pthread_create(&rd[i],NULL,readstart,(void*)rq);
       if(ret<0)
       {
           perror("pthread_creat\n");
           return -1;
       }
         ret = pthread_create(&wr[i],NULL,writestart,(void*)rq);
         if(ret<0)
         {
            perror("pthread_creat\n");
            return -1; 
         }

    }
    for(i=0;i<THREADCOUNT;i++)
    {
        pthread_join(rd[i],NULL);
        pthread_join(wr[i],NULL);
    }
    delete rq;
    return 0;
}
