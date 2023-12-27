#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <cstdio>
#include <cstring>  
#include <thread>

using namespace std;

int tickets = 1000;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex;

// class pData
// {
// public:
//     pData(const string &name, pthread_mutex_t *mutex):_name(name),_pmutex(mutex) 
//     {}
//     ~pData()
//     {}
// public:
//     string _name;
//     pthread_mutex_t *_pmutex;
// };

// void* threadRoutine(void* args)
// {
//     pData *td = static_cast<pData*>(args);
//     while (true)
//     {
//         pthread_mutex_lock(td->_pmutex);
//         if (tickets > 0)
//         {
//             usleep(2000);
//             cout << td->_name << " get a ticket: " << tickets-- << endl;
//             pthread_mutex_unlock(td->_pmutex);
//         }
//         else
//         {
//             pthread_mutex_unlock(td->_pmutex);
//             break;
//         }
//         usleep(1000);
//     }
// }


// int main()
// {
//     pthread_t tids[4];
//     pthread_mutex_t mutex;
//     pthread_mutex_init(&mutex, nullptr);
//     for (int i = 0;i < 4;i++)
//     {
//         char name[64];
//         snprintf(name, 64, "thread %d", i + 1);
//         pData *td = new pData(name, &mutex);
//         pthread_create(tids + i, nullptr, threadRoutine, td);
//     }

//     for (int i = 0;i < 4;i++)
//     {
//         pthread_join(tids[i], nullptr);
//     }
//     pthread_mutex_destroy(&mutex);
//     return 0;
// }



void* threadRoutine(void* args)
{
    string tname = static_cast<const char*>(args);
    while (true)
    {
        if (tickets > 0)
        {
            usleep(2000);
            cout << tname << " get a ticket: " << tickets-- << endl;
        }
        else
        {
            break;
        }
        usleep(1000);
    }
}


int main()
{
    pthread_t tids[4];
    //pthread_mutex_init(&mutex, nullptr);
    for (int i = 0;i < 4;i++)
    {
        char name[64];
        snprintf(name, 64, "thread-%d", i + 1);
        pthread_create(tids + i, nullptr, threadRoutine, name);
    }

    for (int i = 0;i < 4;i++)
    {
        pthread_join(tids[i], nullptr);
    }
    //pthread_mutex_destroy(&mutex);
    return 0;
}




// __thread int g_val = 100;

// void* thread_run(void *args)
// {
//     string name = static_cast<const char*>(args);
//     while (true)
//     {
//         cout << name <<" "<< "g_val:" << g_val++ << " " << "&g_val:" << &g_val << endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     pthread_t t1, t2, t3;
//     pthread_create(&t1, nullptr, thread_run, (void*)"thread-1");
//     pthread_create(&t2, nullptr, thread_run, (void*)"thread-2");
//     pthread_create(&t3, nullptr, thread_run, (void*)"thread-3");

//     pthread_join(t1, nullptr);
//     pthread_join(t2, nullptr);
//     pthread_join(t3, nullptr);

//     return 0;
// }


// void* thread_run(void *args)
// {
//     pthread_detach(pthread_self()); 
//     string name = static_cast<const char*>(args);
//     int cnt = 5;
//     while (cnt--)
//     {
//         cout << name << ":" << cnt << endl;
//         sleep(1);
//     }
//     // pthread_detach(pthread_self());
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, thread_run, (void*)"thread-1");
//     // pthread_detach(tid);

//     sleep(1);
//     int n = pthread_join(tid, nullptr);
//     if (n != 0)
//     {
//         cerr << "errno" << n << ":" << strerror(n) << endl;
//     }
//     // sleep(10);

//     return 0;
// }


// void run1()
// {
//     while (1)
//     {
//         cout << "thread 1" << endl;
//         sleep(1);
//     }
// }

// void run2()
// {
//     while (1)
//     {
//         cout << "thread 2" << endl;
//         sleep(1);
//     }
// }

// void run3()
// {
//     while (1)
//     {
//         cout << "thread 3" << endl;
//         sleep(1);
//     }
// }


// int main()
// {
//     thread t1(run1);
//     thread t2(run2);
//     thread t3(run3);

//     t1.join();
//     t2.join();
//     t3.join();
//     return 0;
// }