#include <iostream>
#include <sys/types.h>
#include <signal.h>
using namespace std;

void Usage(string proc)
{
    cout << "Uage" << endl;
    cout << proc << " signo PID " << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }

    int signo = atoi(argv[1]);
    int PID = atoi(argv[2]);
    kill(PID, signo);
    return 0;
}