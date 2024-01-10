#pragma once 

#include <iostream>
#include <unistd.h>
#include <vector>
#include <queue>
#include "mythread.hpp"
#include "LockGuard.hpp"


using namespace std;

const static int N = 5;

template <class T>
class threadPool
{
public:
    threadPool(const int num = N) :_num(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }
    pthread_mutex_t* getlg()
    {
        return &_mutex;
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
        LockGuard lg(&_mutex);
        _tasks.push(t);
        ThreadWakeup();
    }

    //在回调函数里就对任务栏里的任务进行执行操作，
    static void threadRoutine(void* args)
    {
        pthread_detach(pthread_self());
        threadPool<T>* tp = static_cast<threadPool<T> *>(args);
        while (true)
        {
            T t;
            {
                LockGuard lg(tp->getlg());
                while (tp->Isempty())
                {
                    tp->ThreadWait();
                }
                t = tp->PopTask();
            }
            t();
            cout << "thread handler done, result: " << t.formatRes() << endl;
        }

    }

    void init()
    {
        for (int i = 0;i < _num;i++)
        {
            _threads.push_back(Thread(i, threadRoutine, this));
        }
    }

    void start()
    {
        for (auto& t : _threads)
        {
            t.run();
        }

    }

    ~threadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    vector<Thread> _threads;
    int _num; //线程数量
    queue<T> _tasks;

    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};