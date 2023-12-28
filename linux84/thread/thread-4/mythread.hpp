#pragma once

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <string>

using namespace std;

class Thread
{
public:
    typedef enum
    {
        New = 0,
        Running = 1,
        Exited = 2
    }ThreadStatus;

    typedef void(*func_t)(void*);

    Thread(int num, func_t func, void *args):_tid(0), _status(New), _func(func), _args(args)
    {
        char name[128];
        snprintf(name, 128, "thread-%d", num);
        _tname = name;
    }

    pthread_t threadid()
    {
        if (_status == Running)
        {
            return _tid;
        }
        else
            return 0;
    }

    string threadname()
    {
        return _tname;
    }

    int threadstatus()
    {
        return _status;
    }

    static void* runHelper(void *args)
    {
        Thread* ts = (Thread*)args;
        (*ts)();
        return nullptr;
    }

    void operator()() //仿函数
    {
        if(_func != nullptr) _func(_args);
    }

    void run()
    {
        int n = pthread_create(&_tid, nullptr, runHelper, this);
        if(n != 0) exit(1);
        _status = Running;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        if (n != 0)
        {
            cerr << "main thread join thread " << _tname << " error" << endl;
            return;
        }
        _status = Exited;
    }


    ~Thread()
    {
    }

private:
    pthread_t _tid;
    string _tname;
    func_t _func;
    void* _args;
    ThreadStatus _status;
};