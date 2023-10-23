#include <iostream>
#include <unistd.h>
#include <string>
#include <cassert>
#include <vector>
#include "Task.hpp"
using namespace std;

const int gnum = 5;
Task t;

class EndPoint
{
public:
    pid_t _child_id;
    int _write_fd;
    EndPoint(int id, int fd)
        :_child_id(id),_write_fd(fd)
    {}
    ~EndPoint()
    {}
};


void WaitCommand()
{
    while (true)
    {
        int command = 0;
        int n = read(0, &command, sizeof(int));
        if (n == sizeof(int))
        {
            t.Execute(command);
        }
        else if (n == 0)
        {
            break;
        }
        else
        {
            break;
        }
    }
    }

void CreatProcess(vector<EndPoint> *end_points)
{
    // 1. 先进行构建控制结构，父进程写入，子进程读取
    for (int i = 0; i < gnum; ++i)
    {
        // 1.1 创建管道
        int pipefd[2] = { 0 };
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;

        // 1.2 创建子进程
        pid_t id = fork();
        assert(id >= 0);
        // 子进程
        if (id == 0)
        {
            // 1.3 关闭不需要的读写端
            close(pipefd[1]);

            // 让子进程读取指令时从标准输入读取
            // 输入重定向
            dup2(pipefd[0], 0);
            //子进程开始等待获取指令
            WaitCommand();
    
            close(pipefd[0]);
            exit(0);
        }

        //父进程
        // 1.3 关闭不需要的读写端
        close(pipefd[0]);

        // 1.4 将新的子进程与管道写端构建对象，之后父进程便可以通过 end_points 给子进程“发指令”
        end_points->push_back(EndPoint(id, pipefd[1]));
    }
}


int main()
{
    vector<EndPoint> end_points;
    CreatProcess(&end_points);
    int num = 0;
    while (true)
    {
        //1.选择任务
        int command = Print_Log;
        //2.选择进程
        int index = rand() % end_points.size();
        //3.下发任务
        write(end_points[index]._write_fd, &command, sizeof(command));

        sleep(1);
    }
    return 0;
}