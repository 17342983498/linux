#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "task.hpp"
using namespace std;

const int gnum = 5;
Task t;

class EndPoint
{
private:
    static int number;
public:
    int _write_fd;
    pid_t _child_id;
    string processname;
public:
    EndPoint(int fd, int id)
        :_write_fd(fd), _child_id(id)
    {
        char namebuffer[64];
        snprintf(namebuffer, sizeof(namebuffer), "process-%d[%d:%d]", number++, _child_id, _write_fd);
        processname = namebuffer;
    }
    string name()const
    {
        return processname;
    }
    ~EndPoint()
    {

    }
};

int EndPoint::number = 0;


void waitcommand()
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
            cout << "父进程让我退出，我就退出了: " << getpid() << endl; 
            break;
        }
        else
        {
            break;
        }
    }
}


void createProcess(vector<EndPoint> *end_points)
{
    vector<int> fds;//为了关闭不需要的写端

    for (int i = 0;i < gnum;i++)
    {
        // 1. 创建管道
        int pipefd[2] = { 0 };
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;

        // 2. 创建子进程
        pid_t id = fork();
        assert(id != -1);

        if (id == 0)
        {
            for(auto &fd:fds)
            {
                close(fd);
            }
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            waitcommand();
            close(pipefd[0]);
            exit(0);
        }
        close(pipefd[0]);
        end_points->push_back(EndPoint(pipefd[1],id));
        fds.push_back(pipefd[1]);
    }
}


int ShowBoard()
{
    cout << "##########################################" << endl;
    cout << "|   0. 执行日志任务   1. 执行数据库任务    |" << endl;
    cout << "|   2. 执行请求任务   3. 退出             |" << endl;
    cout << "##########################################" << endl;
    cout << "请选择# ";
    int command = 0;
    cin >> command;
    return command;
}

void ctrlProcess(const vector<EndPoint>& end_points)
{
    int num = 0;
    int cnt = 0;
    while (true)
    {
        // 选择任务
        int command = ShowBoard();
        if(command == 3) break;
        if (command < 0 || command > 2) continue;

        // 选择进程
        int index = cnt++;
        cnt %= end_points.size();
        string name = end_points[index].name();
        cout << "选择了进程: " << name << " | 处理任务: " << command << endl;

        // 下发任务
        write(end_points[index]._write_fd, &command, sizeof(command));

        sleep(1);
    }
}

void waitProcess(const vector<EndPoint> &end_points)
{
    for(int end = 0; end < end_points.size(); end++)
    {
        cout << "父进程让子进程退出:" << end_points[end]._child_id << endl;
        close(end_points[end]._write_fd);

        waitpid(end_points[end]._child_id, nullptr, 0);
        cout << "父进程回收了子进程:" << end_points[end]._child_id << endl;
    } 
    sleep(5);

}



int main()
{
    vector<EndPoint> end_points;
    createProcess(&end_points);
    ctrlProcess(end_points);
    waitProcess(end_points);
}