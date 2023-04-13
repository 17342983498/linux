#include <iostream>
#include <string>
using namespace std;
#include <unistd.h>
#include <vector>
#include <cassert>
#include <sys/wait.h>
#include <sys/types.h>
#include "task.hpp"

const int num=5;
Task t;


int showboard()
{
    cout<<"************************************"<<endl;
    cout<<"*****  0. 日志打印    1. 数据访问*****"<<endl;
    cout<<"*****  2. 网络请求    3. 退出     ****"<<endl;
    cout<<"************************************"<<endl;
    cout<<"请选择#";
    int command = 0;
    cin >> command;
    return command;
}

class End_Point
{
private:
    static int number;
public:
    End_Point(int id,int fd)
    :_child_id(id),_write_fd(fd)
    {
        char namebuffer[64];
        snprintf(namebuffer,sizeof(namebuffer),"process-%d[%d:%d]",number++,_child_id,_write_fd);
        processname=namebuffer;
    }
    string name()const
    {
        return processname;
    }
    ~End_Point()
    {}

public:
    pid_t _child_id;
    int _write_fd;
    string processname;
};

int End_Point::number=0;


//子进程要执行的方法
void WaitCommand()
{
    while(true)
    {
        int command=0;
        int n=read(0,&command,sizeof(int));
        if(n==sizeof(int))
        {
            t.Execute(command);
        }
        else if(n==0)
        {
            cout<<"父进程让我退出，我就退出了！"<<getpid()<<endl;
            break;
        }
        else
        {
            break;
        }
    }
}


void CreatProcesses(vector<End_Point>* end_points)
{
    vector<int> fds;
    for(int i=0;i<num;++i)
    {
        //1.1 构建管道
        int pipefd[2]={0};
        int n=pipe(pipefd);
        assert(n==0);
        (void)n;


        //1.2创建子进程
        pid_t id=fork();
        assert(id!=-1);

        //子进程
        if(id==0)
        {
            for(auto &fd : fds) close(fd);
            //1.3子进程关闭写端
            close(pipefd[1]);
            //1.3.1输出重定向，不让子进程从管道读，而从标准输入读
            dup2(pipefd[0],0);
            //1.3.2子进程开始等待命令
            WaitCommand();
            close(pipefd[0]);
            exit(0);
        }

        //父进程
        //父进程关闭读端
        close(pipefd[0]);

        //将新的子进程与它的管道写端构建对象
        end_points->push_back(End_Point(id,pipefd[1]));
        fds.push_back(pipefd[1]);
    }
}

void CtrlProcess(const vector<End_Point>& end_points)
{
    int num=0;
    int cnt=0;
    while(true)
    {
        int command=showboard();
        if(command == 3) break;
        if(command < 0 || command > 2) continue;
        int index=cnt++;
        cnt%=end_points.size();
        cout<<"选择了进程："<<end_points[index].name()<<"| 处理任务"<<command<<endl;
        

        write(end_points[index]._write_fd,&command,sizeof(command));
        sleep(1);
    }
}

void waitProcess(const vector<End_Point>& end_points)
{
/*     for(const auto &e:end_points)
    {
        close(e._write_fd);
    }
    cout<<"管道写端已关闭，子进程将全部退出！"<<endl;
    sleep(5);
    for(const auto &e:end_points)
    {
        waitpid(e._child_id,nullptr,0);
    }
    cout<<"父进程回收所有的子进程！"<<endl;
    sleep(5); */

/*     for(const auto &e:end_points)//这样的做法不行，原因在于在第二次创建子进程时会将第一次父进程与管道建立连接的方法继承下来，
    //那么第一个管道的写端不仅有父进程，还有管道的读端也会指向第一个管道的写端
    //当你只关闭了第一个父进程的写端，但是第一个管道还有一个写端（第二个管道的读端）并没有关闭
    {
        close(e._write_fd);
        cout<<"管道写端已关闭，子进程将全部退出！"<<endl;
        waitpid(e._child_id,nullptr,0);
        cout<<"父进程回收所有的子进程！"<<endl;
    } */

/*     //可以逆序的关闭父进程的写端（当写端关闭，管道的读端会自动关闭，那么上一个管道的其中一个写端就会关闭，只剩下了一个父进程的写端）
    for(int end=end_points.size()-1;end>=0;end--)
    {
        close(end_points[end]._write_fd);
        cout<<"管道写端已关闭，子进程将全部退出！"<<end_points[end]._child_id<<endl;
        waitpid(end_points[end]._child_id,nullptr,0);
        cout<<"父进程回收所有的子进程！"<<end_points[end]._child_id<<endl;
    } */

    for(int end=0;end<end_points.size();end++)
    {
        close(end_points[end]._write_fd);
        cout<<"管道写端已关闭，子进程将全部退出！"<<end_points[end]._child_id<<endl;
        waitpid(end_points[end]._child_id,nullptr,0);
        cout<<"父进程回收所有的子进程！"<<end_points[end]._child_id<<endl;
    } 


}


int main()
{
    //1.构建控制结构,父进程写入、子进程读取
    vector<End_Point> end_points;
    CreatProcesses(&end_points);
    //来到这里，得到了一个父进程与多个子进程沟通的管道数组

    CtrlProcess(end_points);
    
    waitProcess(end_points);
    
    return 0;
}