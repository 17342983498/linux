#pragma once


#include <iostream>
using namespace std;


class Mutex
{
public:
    Mutex(pthread_mutex_t *pmutex):_pmutex(pmutex)
    {}
    void lock()
    {
        pthread_mutex_lock(_pmutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(_pmutex);
    }
    ~Mutex()
    {}
private:
    pthread_mutex_t *_pmutex;
};


class LockGuard
{
public:
    LockGuard(pthread_mutex_t *mutex) :_mutex(mutex)
    {
        _mutex.lock();
    }
    ~LockGuard()
    {
        _mutex.unlock();
    }
private:
    Mutex _mutex;
};