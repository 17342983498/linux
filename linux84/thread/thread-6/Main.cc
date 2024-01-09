#include "RingQueue.hpp"
#include <ctime>
#include <pthread.h>
#include <unistd.h>
#include "Task.hpp" 
#include <sys/wait.h>


void *ProductRoutine(void* args)
{
    RingQueue<Task>* rq = static_cast<RingQueue<Task> *>(args);
    string res = "+-*/%";
    while (true)
    {
        sleep(1);
        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = res[rand() % res.size()];
        Task t(x, y, op);
        rq->push(t);
        cout << pthread_self() << " product Task: " << t.formatArg() << "?" << endl;
    }
}

void *ConsumeRoutine(void* args)
{
    RingQueue<Task>* rq = static_cast<RingQueue<Task> *>(args);
    int data;
    while (true)
    {
        //sleep(1);
        Task t;
        rq->pop(&t);
        t();
        usleep(100);
        cout << pthread_self() << " consume Task: " << t.formatArg() << t.formatRes() << endl;

    }
}

int main()
{
    srand(time(nullptr) ^ getpid());
    RingQueue<Task> *rq = new RingQueue<Task>();
    pthread_t c[2], p[3];
    for (int i = 0;i < 2;i++)
        pthread_create(c + i, nullptr, ConsumeRoutine, rq);
    for (int i = 0;i < 3;i++)
        pthread_create(p + i, nullptr, ProductRoutine, rq);
    for (int i = 0;i < 3;i++)
        pthread_join(p[i], nullptr);
    for (int i = 0;i < 2;i++)
        pthread_join(c[i], nullptr);

    return 0;
}