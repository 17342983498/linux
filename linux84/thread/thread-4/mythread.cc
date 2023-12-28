#include <unistd.h>
#include <cstdio>
#include <cstring>  
#include "mythread.hpp"
#include "LockGuard.hpp"

int tickets = 1000;                            
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void threadRoutine(void* args)
{
    string message = static_cast<const char*>(args);
    while (true)
    {
        // pthread_mutex_lock(&mutex); // 加锁，是一个让不让你通过的策略
        {
            LockGuard lockguard(&mutex); // RAII 风格的锁
            if (tickets > 0)
            {
                usleep(2000);
                cout << message << " get a ticket: " << tickets-- << endl; // 临界区
            }
            else
            {
                break;
            }
        }
        usleep(1000);
    }
}


// void threadRun(void *args)
// {
//     string message = static_cast<const char *>(args);
//     int cnt = 10;
//     while (cnt)
//     {
//         cout << "我是一个线程, " << message << ", cnt: " << cnt-- << endl;
//         sleep(1);
//     }
// }

int main()
{
    Thread t1(1, threadRoutine, (void*)"hello1");
    Thread t2(2, threadRoutine, (void*)"hello2");
    Thread t3(3, threadRoutine, (void*)"hello3");
    Thread t4(4, threadRoutine, (void*)"hello4");

    t1.run();
    t2.run();
    t3.run();
    t4.run();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.threadstatus() << endl;
    // t1.run();
    // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.threadstatus() << endl;
    // t1.join();
    // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.threadstatus() << endl;

    return 0;
}