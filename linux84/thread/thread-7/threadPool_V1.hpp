#pragma once 

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <queue>

using namespace std;

const static int N = 5;

template <class T>
class threadPool
{
public:
    threadPool(const int num = N) :_num(num), _threads(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }
    void LockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }

    void UnlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }

    void ThreadWait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void ThreadWakeup()
    {
        pthread_cond_signal(&_cond);
    }

    bool Isempty()
    {
        return _tasks.empty();
    }

    T PopTask()
    {
        T t = _tasks.front();
        _tasks.pop();
        return t;
    }

    void PushTask(const T &t)
    {
        LockQueue();
        _tasks.push(t);
        ThreadWakeup();
        UnlockQueue();
    }

    //在回调函数里就对任务栏里的任务进行执行操作，
    static void* threadRoutine(void* args)
    {
        pthread_detach(pthread_self());
        threadPool<T>* tp = static_cast<threadPool<T> *>(args);
        while (true)
        {
            tp->LockQueue();
            while (tp->Isempty())
            {
                tp->ThreadWait();
            }
            T t = tp->PopTask();
            tp->UnlockQueue();

            t();
            cout << "thread handler done, result: " << t.formatRes() << endl;
        }

    }

    void init()
    {}

    void start()
    {
        for (int i = 0;i < _num;i++)
        {
            pthread_create(&_threads[i], nullptr, threadRoutine, this);
        }

    }

    ~threadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    vector<pthread_t> _threads;
    int _num; //线程数量
    queue<T> _tasks;

    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};