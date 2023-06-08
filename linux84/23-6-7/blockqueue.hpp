#pragma once

#include <pthread.h>
#include <iostream>
#include <queue>

// using namespace std;

const int gcap = 5;
template<class T>
class BlockQueue
{
public:
    BlockQueue(const int cap = gcap)
    :_cap(cap)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_consumercond,nullptr);
        pthread_cond_init(&_productorcond,nullptr);
    }
    bool IsFull()
    {
        return _q.size() == _cap;
    }
    bool IsEmpty()
    {
        return _q.empty();
    }
    void push(const T& in)
    {
        //1. 先加锁
        pthread_mutex_lock(&_mutex);

        //2. 如果队列已满，则等待
        while(IsFull())
        {
            pthread_cond_wait(&_productorcond,&_mutex);
        }

        _q.push(in);

        //3. 唤醒对方
        pthread_cond_signal(&_consumercond);
        pthread_mutex_unlock(&_mutex);
    }
    void pop(T *out)
    {
        //1. 先加锁
        pthread_mutex_lock(&_mutex);

        //2. 如果队列已满，则等待
        while(IsEmpty())   
        {
            pthread_cond_wait(&_consumercond,&_mutex);
        }

        *out = _q.front();
        _q.pop();

        //3. 唤醒对方
        pthread_cond_signal(&_productorcond);
        pthread_mutex_unlock(&_mutex);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_consumercond);
        pthread_cond_destroy(&_productorcond);
    }
private:
    std::queue<T> _q;
    int _cap;
    pthread_mutex_t _mutex;
    pthread_cond_t _consumercond;//消费者对应的条件变量，当队列空时就让线程阻塞，等待条变量的改变
    pthread_cond_t _productorcond;
};