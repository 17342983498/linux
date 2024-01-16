#include "lockGuard.hpp"
#include <unistd.h>
#include <string>
#include <unistd.h>
#include "Thread.hpp"

int tickets=1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// void threadRoutine(void *args)
// {
//     std::string message=static_cast<const char*>(args);
//     int cnt=10;
//     while(cnt)
//     {
//         std::cout<<"我是一个线程，我想说："<<message<<" cnt: "<<cnt--<<std::endl;
//         sleep(1);
//     }
// }


// int main()
// {
//     Thread t1(1,threadRoutine,(void *)"大道理都懂,小情绪难控");
//     std::cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << " thread status: " << t1.status() << std::endl;
//     t1.run();
//     std::cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << " thread status: " << t1.status() << std::endl;
//     t1.join();
//     std::cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << " thread status: " << t1.status() << std::endl;
// }


void threadRoutine(void *args)
{
    std::string message = static_cast<const char *>(args);
    while(true)
    {
        LockGuard lockguard(&mutex);
        if (tickets > 0)
        {
            usleep(2000);
            std::cout << message << " get a ticket: " << tickets-- << std::endl; // 临界区
        }
        else
        {
            break;
        }
    }
}



int main()
{
    Thread t1(1,threadRoutine,(void *)"大道理都懂,小情绪难控1");
    Thread t2(2,threadRoutine,(void *)"大道理都懂,小情绪难控2");
    Thread t3(3,threadRoutine,(void *)"大道理都懂,小情绪难控3");
    Thread t4(4,threadRoutine,(void *)"大道理都懂,小情绪难控4");

    t1.run();
    t2.run();
    t3.run();
    t4.run();

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}