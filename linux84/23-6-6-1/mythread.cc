#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <cstdio>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
const int num=5;



void *active(void *args)
{
    string name=static_cast<const char*>(args);
    while(true)
    {
        pthread_mutex_lock(&mutex);
        //功能：先让所有的线程都进入等待序列
        pthread_cond_wait(&cond,&mutex);//在调用的时候会自动释放锁
        cout << name << " 活动" << endl;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}



int main()
{
    pthread_t tids[num];
    for(int i=0;i<num;++i)
    {
        char *name=new char[32];
        snprintf(name,32,"thread-%d",i+1);
        pthread_create(tids+i,nullptr,active,name);
    }

    sleep(5);
    while(true)
    {
        cout<<"main thread wakeup thread..."<<endl;
        //pthread_cond_broadcast(&cond);//广播唤醒
        pthread_cond_signal(&cond);
        sleep(1);
    }


    for(int i=0;i<num;++i)
    {
        pthread_join(tids[i],nullptr);
    }
    return 0;
}





// void *ThreadRoutine(void *args)
// {
//     std::cout<<"i am a new thread"<<std::endl;
//     pthread_mutex_lock(&mutex);
//     std::cout<<"i got a mutex"<<std::endl;

//     pthread_mutex_lock(&mutex);
//     std::cout<<"i alive again"<<std::endl;
    
//     return nullptr;
// }



// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid,nullptr,ThreadRoutine,nullptr);
//     sleep(5);

//     std::cout<<"main thread run begin"<<std::endl;
//     pthread_mutex_unlock(&mutex);
//     std::cout<<"main thread unlock success"<<std::endl;
//     sleep(5);
//     return 0;
// }