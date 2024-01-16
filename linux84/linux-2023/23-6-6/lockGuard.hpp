#include <iostream>
using namespace std;
#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t *mutex)
    :_mutex(mutex)
    {}
    ~Mutex()
    {}
    void lock()
    {
        pthread_mutex_lock(_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(_mutex);
    }
private:
    pthread_mutex_t *_mutex;
};


class LockGuard
{
public:
    LockGuard(pthread_mutex_t *mutex)
    :_mutex(mutex)
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