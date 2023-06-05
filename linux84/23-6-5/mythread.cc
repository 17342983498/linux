#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

using namespace std;

int tickets=10000;
//互斥锁
// pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


//凡是访问同一个临界资源的线程，都需要进行加锁，而且必须是同一把锁，这是规则
//加锁的本质是给临界区加锁
//加锁前所有线程必须都先看到同一把锁，而锁本身就是公共资源->锁如何保证自己的安全呢？->加锁和解锁本身是原子的

class TData
{
public:
    TData(string name,pthread_mutex_t *pmutex)
    :_name(name)
    ,_pmutex(pmutex)
    {}
    ~TData()
    {}
public:
    string _name;
    pthread_mutex_t *_pmutex;
};


void *Routine_Run(void *args)
{
    TData *td = static_cast<TData *>(args);
    while(true)
    {
        pthread_mutex_lock(td->_pmutex);
        if(tickets>0)
        {
            //模拟抢票所花的时间
            usleep(2000);
            cout<<td->_name<<" get a ticket: "<<tickets--<<endl;
            pthread_mutex_unlock(td->_pmutex);
        }
        else
        {
            pthread_mutex_unlock(td->_pmutex);
            break;
        }
        usleep(1000);
    }
    return nullptr;
}

int main()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,nullptr);
    pthread_t tid[4];
    int n=sizeof(tid)/sizeof(tid[0]);
    for(int i=0;i<n;++i)
    {
        char name[64];
        snprintf(name,64,"thread-%d",i+1);
        TData *td=new TData(name,&mutex);
        pthread_create(tid+i,nullptr,Routine_Run,td);
    }

    for(int i=0;i<n;++i)
    {
        pthread_join(tid[i],nullptr);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}