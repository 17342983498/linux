#ifndef __COMM_HPP__
#define __COMM_HPP__
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <cerrno>
#include <string.h>
#include <string>
#include <cstdio>
#include <cassert>

using namespace std;


#define PATHNAME "."  //表示当前路径
#define PROJID 0x666
#define gsize 4096

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
    return creatShmHelper(key,size,IPC_CREAT|IPC_EXCL);
}

int getShm(key_t key,int size)
{
    return creatShmHelper(key,size,IPC_CREAT);
}

void delShm(int shmid)
{
    int n=shmctl(shmid,IPC_RMID,nullptr);
    assert(n!=-1);
    (void)n; 
}




#endif