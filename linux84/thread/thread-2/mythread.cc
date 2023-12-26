#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

using namespace std;

const int NUM = 10;


class ThreadData
{
public:
    ThreadData(string name, int createtime, int id, int num, int sum) :_name(name), _createtime(createtime), _id(id), _num(num), _sum(sum)
    {}
    ~ThreadData()
    {}    
public:
    string _name;
    int _createtime;
    int _id;
    int _num;
    int _sum;
};

void* thread_run(void* args) 
{
    //char* name = (char*)args;
    ThreadData *td = static_cast<ThreadData *> (args);
    while (true)
    {
        for (int i = 1;i < td->_num; i++)
        {
            td->_sum += i;
        }
        cout << td->_name <<" cal down!" << endl;
        break;
        // cout << "new thread is running! my thread name is " << td->_name << " create time is " << td->_createtime << " index is " << td->_id << endl;
        // sleep(1);
        // break; //正常退出
        // // exit(1);// 整个进程退出

        // // 只退出0号线程
        // // if (strcmp(name, "thread-0") == 0)
        // // {
        // //     pthread_exit(nullptr);
        // // }
    }
    //delete td;
    pthread_exit(td);
    //delete name;
    //return nullptr;
}


int main()
{
    pthread_t tids[NUM];
    for (int i = 0;i < NUM;i++)
    {
        char name[64];
        snprintf(name, 64, "thread-%d", i);
        ThreadData* td = new ThreadData(name, time(nullptr), i + 1, 100 + i, 0);
        sleep(1);
        pthread_create(tids + i, nullptr, thread_run, td);
    }

    // 线程等待
    void* value_ptr = nullptr;
    for (int i = 0;i < NUM;i++)
    {
        int ret = pthread_join(tids[i], &value_ptr);
        if (ret != 0) cerr << "pthread_join error" << endl;

        ThreadData* td = static_cast<ThreadData*>(value_ptr);
        cout << td->_name << " 计算结果为：" << td->_sum << " (它要计算的是[1, " << td->_num << "])" << endl;
        //cout << "The thread exit information is " << (uint64_t)value_ptr << endl;
    }

    cout << "All threads exit... " << endl;

    // while (true)
    // {
    //     cout << "i am main thread..." << endl;
    //     sleep(1);
    // }
    // sleep(3);
    return 0;
}