#pragma once

#include <iostream>
#include <semaphore.h>
#include <vector>
using namespace std;

static int N = 5;

template<class T>
class RingQueue
{
public:
    RingQueue(const int num = N) :_q(num), _cap(num)
    {
        sem_init(&_data_sem, 0, 0);
        sem_init(&_space_sem, 0, _cap);
        pthread_mutex_init(&_p_mutex, nullptr);
        pthread_mutex_init(&_c_mutex, nullptr);
        _pro_step = _con_step = 0;
    }
    void push(const T& in)
    {
        sem_wait(&_space_sem);
        pthread_mutex_lock(&_p_mutex);
        _q[_pro_step++] = in;
        _pro_step %= _cap;
        pthread_mutex_unlock(&_p_mutex);
        sem_post(&_data_sem);
    }
    void pop(T* out)
    {
        sem_wait(&_data_sem);
        pthread_mutex_lock(&_c_mutex);
        *out = _q[_con_step++];
        _con_step %= _cap;
        pthread_mutex_unlock(&_c_mutex);
        sem_post(&_space_sem);
    }
    ~RingQueue()
    {
        sem_destroy(&_data_sem);
        sem_destroy(&_space_sem);
        pthread_mutex_destroy(&_p_mutex);
        pthread_mutex_destroy(&_c_mutex);
    }
private:
    vector<T> _q;
    int _cap;
    sem_t _data_sem;
    sem_t _space_sem;
    int _pro_step;
    int _con_step;
    pthread_mutex_t _p_mutex;
    pthread_mutex_t _c_mutex;
};