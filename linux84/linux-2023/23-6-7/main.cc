#include "blockqueue.hpp"
#include "task.hpp"
#include <ctime>
#include <unistd.h>
#include <pthread.h>

void *consumer(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while(true)
    {
        //sleep(1);
        Task t;
        bq->pop(&t);
        t();
        std::cout << pthread_self() << " | consumer data: " << t.formatArg() << t.formatRes() << std::endl;
    }
}

void *productor(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    std::string opers = "+-*/%";
    while(true)
    {
        //先生成数据
        //sleep(1);
        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = opers[rand() % opers.size()];

        //将数据推送到blockqueue中
        Task t(x, y, op);
        bq->push(t);

        // sleep(1);
        // int data = rand()%10+1;
        // bq->push(data);

        std::cout << pthread_self() << " | productor Task: " <<  t.formatArg() << "?" << std::endl;
    }
}

int main()
{
    srand((uint64_t)time(nullptr) ^ getpid());
    BlockQueue<int> *bq = new BlockQueue<int>(); 
    // pthread_t co,pr;
    // pthread_create(&co,nullptr,consumer,bq);
    // pthread_create(&pr,nullptr,productor,bq);

    // pthread_join(co,nullptr);
    // pthread_join(pr,nullptr);

    pthread_t c[2], p[3];
    pthread_create(&c[0], nullptr, consumer, bq);
    pthread_create(&c[1], nullptr, consumer, bq);
    pthread_create(&p[0], nullptr, productor, bq);
    pthread_create(&p[1], nullptr, productor, bq);
    pthread_create(&p[2], nullptr, productor, bq);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    pthread_join(p[2], nullptr);

    delete bq;
    return 0;
}