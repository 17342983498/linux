#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

void *thread1(void *arg)
{
    while (1)
    {
        cout << "I am thread1, my pid is " << getpid() << endl;
        sleep(1);
    }
}

void *thread2(void *arg)
{
    while (1)
    {
        cout << "I am thread2, my pid is " << getpid() << endl;
        sleep(1);
    }
}

void *thread3(void *arg)
{
    while (1)
    {
        cout << "I am thread3, my pid is " << getpid() << endl;
        sleep(1);
    }
}


int main()
{
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_create(&tid3, NULL, thread3, NULL);

    while(1)
    {
        printf("我是主线程， 我正在运行\n");
        sleep(1);
    }
    return 0;
}