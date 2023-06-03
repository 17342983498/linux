#include <iostream>
#include <pthread.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <unistd.h>

using namespace std;

int tickets=1000;
pthread_mutex_t mutex;

void *threadRoutine(void *name)
{
    string tname=static_cast<const char*>(name);
    while(true)
    {
        pthread_mutex_lock(&mutex);
        if(tickets>0)
        {
            usleep(2000);
            cout<<tname<<" get a ticket "<<tickets--<<endl;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        //模拟将抢到的票给到顾客，避免同一个人一直加锁解锁
        usleep(1000);
    }
    return nullptr;
}


int main()
{
    pthread_t tid[5];
    int num=sizeof(tid)/sizeof(tid[0]);
    for(int i=0;i<num;++i)
    {
        char *name=new char[64];
        snprintf(name,64,"thread-%d",i+1);
        pthread_create(tid+i,nullptr,threadRoutine,(void*)name);
    }

    for(int i=0;i<num;++i)
    {
        pthread_join(tid[i],nullptr);
    }
    return 0;
}


// __thread int g_val = 10;

// void *threadRoutine(void *args)
// {
//     string name = static_cast<const char*>(args);
//     int cnt=5;
//     while(cnt--)
//     {
//         cout<<name<<" g_val: "<< g_val++ <<" , &g_val: "<< &g_val <<endl;
//         sleep(1);
//     }
//     return nullptr;
// }


// int main()
// {
//     pthread_t t1,t2,t3;
//     pthread_create(&t1, nullptr, threadRoutine, (void*)"thread 1");
//     pthread_create(&t2, nullptr, threadRoutine, (void*)"thread 2");
//     pthread_create(&t3, nullptr, threadRoutine, (void*)"thread 3");


//     pthread_join(t1,nullptr);
//     pthread_join(t1,nullptr);
//     pthread_join(t1,nullptr);
//     return 0;
// }






// void *thread_run(void *args)
// {
//     //sleep(1);
//     //pthread_detach(pthread_self());
//     string name = static_cast<const char*> (args);
//     int cnt=5;
//     while(cnt)
//     {
//         cout<<name<<" : "<<cnt--<<endl;
//         sleep(1);
//     }
//     return nullptr;
// }


// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid,nullptr,thread_run,(void*)"thread 1");

//     //pthread_detach(tid);
//     //sleep(1);
//     int n=pthread_join(tid,nullptr);
//     if(n!=0)
//     {
//         cerr<<"error: "<< n <<" : "<< strerror(n) <<endl;
//     }
//     return 0;
// }