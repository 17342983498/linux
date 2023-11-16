#include <iostream>
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "comm.hpp"

using namespace std;


int main()
{
    umask(0);
    int n = mkfifo(filename.c_str(), mode);
    if (n != 0)
    {
        cout << errno << " : " << strerror(errno) << endl;
        return 1;
    }
    cout << "create pipe successful!" << endl;

    int rfd = open(filename.c_str(), O_RDONLY);
    if (rfd < 0)
    {
        cout << errno << " : " << strerror(errno) << endl;
        return 2;
    }
    cout << "open fifo success, begin ipc!" << endl;

    char buffer[1024];
    while (true)
    {
        int n = read(rfd, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = 0;
            cout << "client# " << buffer << endl;
        }
        else if (n == 0)
        {
            cout << "client quit,me too!" << endl;
            break;
        }
        else
        {
            cout << errno << " : " << strerror(errno) << endl;
            return 3;
        }

    }
    close(rfd);
    unlink(filename.c_str());
}