#pragma once

#include <iostream>
#include <functional>
#include <vector>

typedef void (*func_t)();


#define COMMAND_LOG 0
#define COMMAND_VISIT 1
#define COMMAND_REQEUST 2


void PrintLog()
{
    std::cout<<"执行日志打印任务!!!!\n"<<std::endl;
}

void DataVisit()
{
    std::cout<<"执行数据访问任务!!!!\n"<<std::endl;
}

void NetRequest()
{
    std::cout<<"执行网络请求任务!!!!\n"<<std::endl;
}


class Task
{
public:
    Task()
    {
        funcs.push_back(PrintLog);
        funcs.push_back(DataVisit);
        funcs.push_back(NetRequest);
    }
    void Execute(int command)
    {
        if(command>=0&&command<funcs.size())
        {
            funcs[command]();
        }
    }
    ~Task()
    {}
public:
    std::vector<func_t> funcs;
};