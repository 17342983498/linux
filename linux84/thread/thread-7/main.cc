#include "threadPool_V3.hpp"
#include "task.hpp"
#include <memory>

int main()
{
    unique_ptr<threadPool<Task>> tp(new threadPool<Task>());
    tp->init();
    tp->start();

    while (true)
    {
        int x, y;
        char op;
        std::cout << "please Enter x> ";
        std::cin >> x;
        std::cout << "please Enter y> ";
        std::cin >> y;
        std::cout << "please Enter op(+-*/%)> ";
        std::cin >> op;

        Task t(x, y, op);
        tp->PushTask(t);
        sleep(1);
    }
}