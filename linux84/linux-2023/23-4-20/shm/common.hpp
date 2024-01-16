#ifndef __COMM_HPP__
#define __COMM_HPP__
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>
#include <string>
#include <cstdio>
#include <cassert>

using namespace std;


#define PATHNAME "."  //表示当前路径
#define PROJID 0x666
const int gsize = 4097;

key_t getKey()
{
    key_t k=ftok(PATHNAME,PROJID);
    if(k==-1)
    {
        cerr<<errno<<":"<<strerror(errno)<<endl;
        exit(1);
    }
    return k;
}



string toHex(int x)
{
    char buf[64];
    snprintf(buf,sizeof(buf),"0x%x",x);
    return buf;
}



static int creatShmHelper(key_t k,int size,int flag)
{
    int shmid=shmget(k,gsize,flag);
    if(shmid==-1)
    {
        cerr<<errno<<":"<<strerror(errno)<<endl;
        exit(2);
    }
    return shmid;
} 

int creatShm(key_t key,int size)
{
    return creatShmHelper(key,size,IPC_CREAT|IPC_EXCL|0666);
}

int getShm(key_t key,int size)
{
    return creatShmHelper(key,size,IPC_CREAT);
}

char* attachShm(int shmid)
{
    char* start=(char*)shmat(shmid,nullptr,0);
    return start;
}

void detachShm(char* start)
{
    int n=shmdt(start);
    assert(n!=-1);
    (void)n;
}

void delShm(int shmid)
{
    int n=shmctl(shmid,IPC_RMID,nullptr);
    assert(n!=-1);
    (void)n; 
}


#define SERVER 1
#define CLIENT 0

class Init
{
public:
    Init(int t):Type(t)
    {
        key_t k =getKey();
        if(Type==SERVER)
        {
            shmid = creatShm(k,gsize);
        }
        else
            shmid=getShm(k,gsize);
        start=attachShm(shmid);
    }
    char* getStart()
    {
        return start;
    }
    ~Init()
    {
        detachShm(start);
        if(Type==SERVER)
            delShm(shmid);
    }
private:
    char* start;
    int shmid;
    int Type;
};




#endif