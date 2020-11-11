#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <semaphore.h>

#define CAPACITY 1
#define threadcount 1 
class SingQueue
{
public:
    SingQueue()
        :v_(CAPACITY)
    {
        capacity_=CAPACITY;
        pos_read_ = 0;
        pos_write_ = 0;

        sem_init(&lock_,0,1);
        sem_init(&write_,0,CAPACITY);
        sem_init(&read_,0,0);
    }
    ~SingQueue()
    {
        sem_destroy(&write_);
        sem_destroy(&read_);
    }
    void push(int data)
    {
        sem_wait(&write_);
        sem_wait(&lock_);
        v_[pos_write_]=data;
        pos_write_=(pos_write_+1)%CAPACITY;
        sem_post(&lock_);
        sem_post(&read_);
    }
    void pop(int* data)
    {
        sem_wait(&read_);
        sem_wait(&lock_);
        *data = v_[pos_read_];
        pos_read_= (pos_read_+1)%capacity_;
        sem_post(&lock_);
        sem_post(&write_);
    }
private:
    std::vector<int> v_;
    int capacity_;
    int pos_write_;
    int pos_read_;

    sem_t read_;
    sem_t write_;
    sem_t lock_;

};

void* readstart(void* arg)
{
    SingQueue* rq = (SingQueue*)arg;
    while(1)
    {
        int data;
        rq->pop(&data);
        printf("i am %p,i read %d\n",pthread_self(),data);
    }
    return NULL;

}
void* writestart(void* arg)
{
    SingQueue* rq = (SingQueue*)arg;
    int data = 0;
    while(1)
    {
        rq->push(data);
        printf("i am %p,i write %d\n",pthread_self(),data);
        data++;
    }
    return NULL;
}

int main()
{
    pthread_t rd[threadcount],wr[threadcount];
    SingQueue* rq = new SingQueue;
    int i = 0;
    for(int i=0;i<threadcount;i++)
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
    for(i=0;i<threadcount;i++)
    {
        pthread_join(rd[i],NULL);
        pthread_join(wr[i],NULL);
    }
    delete rq;
    return 0;
}
