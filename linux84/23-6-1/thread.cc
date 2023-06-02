#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <ctime>
using namespace std;



void *thread_test(void *args)
{
    const char* name=static_cast<const char*>(args);
    int cnt=5;
    while(cnt)
    {
        cout<<name<<" is running: "<<cnt--<<" obtain self id: "<<pthread_self()<<endl;
        sleep(1);
    }
    pthread_exit((void*)11);
}




int main()
{
    pthread_t tid;
    pthread_create(&tid,nullptr,thread_test,(void*)"thread 1");
    sleep(3);
    pthread_cancel(tid);

    void *ret=nullptr;
    pthread_join(tid,&ret);
    cout<<"new thread exit: "<<(uint64_t) ret<<"quit thread:"<<tid<<endl;
    return 0;
}







// int NUM = 10;

// enum {OK=0,ERROR};

// class ThreadData
// {
// public:
//     ThreadData(const string& name, int ID, time_t createTime,int top)
//     :_name(name),_ID(ID),_createTime((uint64_t)createTime),_status(OK),_top(top),_result(0)
//     {}
//     ~ThreadData()
//     {}
// public:
//     string _name;
//     int _ID;
//     uint64_t _createTime;

//     int _status;
//     int _top;
//     int _result;
// };




// void *thread_run(void *args)
// {
//     //char* name=(char*) args;
//     ThreadData *td = static_cast<ThreadData *>(args);
//     // while(true)
//     // {
//     //     cout<<"i am new thread, name:  "<< td->_name<<"  create time:  "<<td->_createTime<<"  index:  "<<td->_ID <<endl;
//     //     sleep(4);
//     //     break;
//     // }
    
//     for(int i=0;i<td->_top;++i)
//     {
//         td->_result+=i;
//     }
//     cout<<td->_name<<"cal done"<<endl;   

//     //delete td;
//     pthread_exit(td);
//     //return nullptr;
// }


// int main()     
// {
//     pthread_t tids[NUM];
//     for(int i=0;i<NUM;++i)
//     {
//         //char *tname = new char[64];
//         char tname[64];
//         snprintf(tname,64,"thread-%d",i+1);
//         ThreadData* td=new ThreadData(tname,i+1,time(nullptr),100+i*5);
//         pthread_create(tids+i, nullptr, thread_run, td);
//         sleep(1);
//     }
//     void *ret = nullptr;
//     for(int i=0;i<NUM;++i)
//     {
//         int t = pthread_join(tids[i],&ret);
//         if(t!=0)
//             cerr<<"pthread_join fail"<<endl;
//         //cout<<"thread quit:"<<(uint64_t)ret<<endl;
//         ThreadData *td = static_cast<ThreadData *>(ret);
//         if(td->_status==OK)
//         {
//             cout<<td->_name<<" 计算结果是："<<td->_result<<"它要计算的是【1, "<<td->_top<<"】"<<endl;
//         }
//         delete td;
//     }

//     cout<<"all thread quit..."<<endl;


//     //sleep(3);
//     // while(true)
//     // {
//     //     cout<<"i am main thread"<<endl;
//     //     sleep(1);
//     // }
//     return 0;
// }