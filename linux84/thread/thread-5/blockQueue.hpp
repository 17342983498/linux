#pragma once

#include <iostream>
#include <pthread.h>
#include <ctime>
#include <queue>

using namespace std;

const int gnum = 5;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


template<class T>
class BlockQueue
{
public:
    BlockQueue(const int cap = gnum):_cap(cap)
    { 
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_Product_cond, nullptr);
        pthread_cond_init(&_Consume_cond, nullptr);
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
        pthread_mutex_lock(&_mutex);
        while (IsFull())
        {
            pthread_cond_wait(&_Product_cond, &_mutex);
        }
        _q.push(in);
        pthread_cond_signal(&_Consume_cond);
        pthread_mutex_unlock(&_mutex);
    }

    void pop(T* out)
    {
        pthread_mutex_lock(&_mutex);
        while (IsEmpty())
        {
            pthread_cond_wait(&_Consume_cond, &_mutex);
        }
        *out = _q.front();
        _q.pop();
        pthread_cond_signal(&_Product_cond);
        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_Product_cond);
        pthread_cond_destroy(&_Consume_cond);
    }
private:
    queue<T> _q;
    int _cap;
    pthread_mutex_t _mutex;
    pthread_cond_t _Product_cond;
    pthread_cond_t _Consume_cond;
};
