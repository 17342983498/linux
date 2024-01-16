
#include <iostream>
#include <vector>
#include <string>
#include <semaphore.h>

static const int N = 5;

template<class T>
class RingQueue
{
public:
    RingQueue(int num = N)
    :_cap(N),_v(N)
    {
        sem_init(&_data_sem, 0, 0);
        sem_init(&_space_sem, 0, N);
        _c_step = _p_step = 0;
        pthread_mutex_init(&_c_mutex,nullptr);
        pthread_mutex_init(&_p_mutex,nullptr);
    }
    void Lock(pthread_mutex_t& m)
    {
        pthread_mutex_lock(&m);
    }
    void Unlock(pthread_mutex_t& m)
    {
        pthread_mutex_unlock(&m);
    }
    void push(const T& in)
    {
        sem_wait(&_space_sem);
        Lock(_c_mutex);
        _v[_c_step++]=in;
        _c_step%=_cap;
        Unlock(_c_mutex);
        sem_post(&_data_sem);
    }
    void pop(T *out)
    {
        sem_wait(&_data_sem);
        Lock(_p_mutex);
        *out=_v[_p_step++];
        _p_step%=_cap;
        Unlock(_p_mutex);
        sem_post(&_space_sem);
    }
    ~RingQueue()
    {
        sem_destroy(&_data_sem);
        sem_destroy(&_space_sem);
        pthread_mutex_destroy(&_c_mutex);
        pthread_mutex_destroy(&_p_mutex);
    }
private:
    std::vector<T> _v;
    int _cap;
    sem_t _data_sem;
    sem_t _space_sem;
    int _c_step;
    int _p_step;
    pthread_mutex_t _c_mutex;
    pthread_mutex_t _p_mutex;
};