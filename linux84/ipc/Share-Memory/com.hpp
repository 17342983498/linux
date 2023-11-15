#ifndef __COMM_HPP__
#define __COMM_HPP__

#include <iostream>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;


#define PATHNAME "."
#define PROJID 0x6666

const int gsize = 1024;


key_t getKey()
{
    key_t k = ftok(PATHNAME, PROJID);
    if(k == -1)
    {
        cerr << "error: " << errno << " : " << strerror(errno) << endl;
        exit(1);
    }
    return k;
}

string toHex(int x)
{
    char buffer[64];
    snprintf(buffer, sizeof buffer, "0x%x", x);
    return buffer;
}

static int createShmHelper(key_t k, int size, int shmflag)
{
    int shmid = shmget(k, size, shmflag);
    if(shmid == -1)
    {
        cerr << "error: " << errno << " : " << strerror(errno) << endl;
        exit(2);
    }
    return shmid;
}


int createShm(key_t k, int size)
{
    umask(0);
    return createShmHelper(k, size, IPC_CREAT|IPC_EXCL|0666);
}


int getShm(key_t k, int size)
{
    return createShmHelper(k, size, IPC_CREAT);
}

char* attachShm(int shmid)
{
    char* start = (char*)shmat(shmid, nullptr, 0);
    return start;
}

void detachShm(char* start)
{
    int n = shmdt(start);
    assert(n!=-1);
    (void)n;
}

void delShm(int shmid)
{
    int n = shmctl(shmid, IPC_RMID, nullptr);
    assert(n!=-1);
    (void)n; 
}


#define SERVER 1
#define CLIENT 0


class Init
{
public:
    Init(int t):type(t)
    {
        key_t k = getKey();
        if (type == SERVER)
        {
            shmid = createShm(k, gsize);
        }
        else
        {
            shmid = getShm(k, gsize);
        }
        start = attachShm(shmid);
    }
    char* getStart()
    {
        return start;
    }
    ~Init()
    {
        detachShm(start);
        if(type == SERVER) delShm(shmid);
    }
private:
    int type;
    char* start;
    int shmid;
};




#endif