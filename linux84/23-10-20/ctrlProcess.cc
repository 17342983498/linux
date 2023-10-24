#include <iostream>
#include <unistd.h>
#include <string>
#include <cassert>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include "Task.hpp"
using namespace std;

const int gnum = 5;
Task t;

class EndPoint
{
private:
    static int number;
public:
    pid_t _child_id;
    int _write_fd;
    string process_name;
    EndPoint(int id, int fd)
        :_child_id(id),_write_fd(fd)
    {
        char namebuf[64];
        snprintf(namebuf, sizeof(namebuf), "process-%d[%d:%d]", number++, _child_id, _write_fd);
        process_name = namebuf;
    }
    string name()const
    {
        return process_name;
    }
        ~EndPoint()
    {}
};

int EndPoint::number = 0;

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
    vector<int> fds;
    // 1. 先进行构建控制结构，父进程写入，子进程读取
    for (int i = 0; i < gnum; ++i)
    {
        // 1.1 创建管道
        int pipefd[2] = { 0 };
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;
        for (auto& fd : fds)
        {
            close(fd);
        }

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
        fds.push_back(pipefd[1]);
    }
}

int ShowBoard()
{
    cout << "******************************" << endl;
    cout << "****** 1. 打印  2. 更新 ******" << endl;
    cout << "****** 3. 删除  4. 退出 ******" << endl;
    cout << "******************************" << endl;
    int command = 0;
    cout << "请选择您要执行的任务：";
    cin >> command;
    return command;
}

void CtrlProcess(const vector<EndPoint> &end_points)
{
    int num = 0;
    int cnt = 0;
    while(true)
    {
        //1.选择任务
        int command = ShowBoard();
        if (command == 4)
        {
            break;
        }
        if (command < 1 || command > 3)
        { 
            continue;
        }
            //2.选择进程
        int index = cnt++;
        index %= gnum;
        string name = end_points[index].name();
        cout << "我选择了进程：" << name << " | 处理任务：" << command << endl;
        //3.下发任务
        write(end_points[index]._write_fd, &command, sizeof(command));

        sleep(1);
    }
}

void WaitProcess(const vector<EndPoint>& end_points)
{

    for (const auto& ep : end_points)
    {
        close(ep._write_fd);
        cout << "子进程都已退出！" << ep._child_id << endl;
        waitpid(ep._child_id, nullptr, 0);
        cout<< "父进程已经回收所有子进程！" << ep._child_id <<endl;
    }
    sleep(3);


    // for (const auto& ep : end_points)
    // {
    //     close(ep._write_fd);
    // }
    // cout << "所有子进程都已退出！" << endl;
    // sleep(3);
    // for (const auto& ep : end_points)
    // {
    //     waitpid(ep._child_id, nullptr, 0);
    // }
    // cout<< "父进程已经回收所有子进程！" <<endl;
    // sleep(3);
}


int main()
{
    vector<EndPoint> end_points;
    CreatProcess(&end_points);
    CtrlProcess(end_points);
    WaitProcess(end_points);
    return 0;
}