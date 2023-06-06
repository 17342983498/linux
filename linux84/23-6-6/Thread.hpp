#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>


class Thread
{
public:
    typedef enum
    {
        NEW=0,
        RUNNING,
        EXITED
    }threadstatus;
    typedef void (*func_t)(void *);
public:
    Thread(int num,func_t func,void *args)
    :_tid(0),_func(func),_args(args),_status(NEW)
    {
        char name[128];
        snprintf(name,sizeof(name),"thread-%d",num);
        _name=name;
    }
    int status()
    {
        return _status;
    }
    std::string threadname()
    {
        return _name;
    }
    int threadid()
    {
        return _tid;
    }

    static void *runHelper(void *args)
    {
        Thread *ts=(Thread*)args;
        (*ts)();//调用仿函数
        return nullptr;
    }

    bool operator()()//仿函数
    {
        if(_func != nullptr) _func(_args);
    }

    void run()
    {
        int n=pthread_create(&_tid,nullptr,runHelper,this);
        if(n!=0)
        {
            exit(1);
        }
        _status=RUNNING;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        if( n != 0)
        {
            std::cerr << "main thread join thread " << _name << " error" << std::endl;
            return;
        }
        _status = EXITED;
    }

    ~Thread()
    {}

public:
    pthread_t _tid;
    std::string _name;
    func_t _func;
    void *_args;
    threadstatus _status;
};