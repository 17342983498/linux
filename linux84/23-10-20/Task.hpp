#pragma once

#include <iostream>
using namespace std;
#include <vector>

typedef void (*func_t)();


void PrintLog()
{
    cout << "我是打印日志，我正在执行..." << endl;
}

void UpdateLog()
{
    cout << "我是更新日志，我正在执行..." << endl;
}

void DeleteLog()
{
    cout << "我是删除日志，我正在执行..." << endl;
}

#define PrintLog 0
#define UpdateLog 1
#define DeleteLog 2

class Task
{
public: 
    Task()
    {
        funcs.push_back(PrintLog);
        funcs.push_back(UpdateLog);
        funcs.push_back(DeleteLog);
    }
    void Execute(int command)
    {
        if (command >= 0 && command < funcs.size())
        {
            funcs[command]();
        }
    }
    ~Task()
    {}
public:
    vector<func_t> funcs;
};



