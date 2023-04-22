#include <iostream>
#include <signal.h>
#include <unistd.h>
int main()
{
    //signal(2,handler);

    while(1)
    {
        std::cout<< "我正在运行,我的pid是:"<<getpid()<<std::endl;
        sleep(1);
    }
    return 0;
} 