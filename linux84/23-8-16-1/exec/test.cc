#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
    for(int i = 0; i < 5; i++){
        cout << "-------------------------------------------------------------------------"<< endl;
        cout << "我是另一个程序，我的pid是: " << getpid() << endl;
        cout << " MYENV: " << (getenv("MYENV")==NULL?"NULL":getenv("MYENV")) << endl;
        cout << " PATH: " << (getenv("PATH")==NULL?"NULL":getenv("PATH")) << endl;
        cout << "-------------------------------------------------------------------------"<< endl;
        sleep(1);
    }

    return 0;
}