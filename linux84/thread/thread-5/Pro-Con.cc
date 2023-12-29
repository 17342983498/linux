#include "blockQueue.hpp"
#include "task.hpp"
#include <unistd.h>

void *Product(void *args)
{
    BlockQueue<Task>* bq = static_cast<BlockQueue<Task> *>(args);
    string res = "+-*/%";
    while (true)
    {
        //sleep(1);
        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = res[rand() % res.size()];
        Task t(x, y, op);
        bq->push(t);
        cout << pthread_self() << " product Task: " << t.formatArg() << "?" << endl;

    }
}

void *Consume(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while (true)
    {
        //sleep(1);
        Task t;
        bq->pop(&t);
        t();
        usleep(100);
        cout << pthread_self() << " consume Task: " << t.formatArg() << t.formatRes() << endl;
    }
}

int main()
{
    srand((uint64_t)time(nullptr) ^ getpid());
    //BlockQueue<int> *bq = new BlockQueue<int>();
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t productor[3], consumer[2];
    pthread_create(productor, nullptr, Product, bq);
    pthread_create(productor + 1, nullptr, Product, bq);
    pthread_create(productor + 2, nullptr, Product, bq);
    pthread_create(consumer, nullptr, Consume, bq);
    pthread_create(consumer + 1, nullptr, Consume, bq);

    pthread_join(productor[0], nullptr);
    pthread_join(productor[1], nullptr);
    pthread_join(productor[2], nullptr);
    pthread_join(consumer[0], nullptr);
    pthread_join(consumer[1], nullptr);

    delete bq;

    return 0;
}