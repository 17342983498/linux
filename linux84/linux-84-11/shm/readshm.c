#include<stdio.h>
#include<sys/shm.h>
#include<unistd.h>


#define KEY 0x99999999
int main()
{
    int shmid = shmget(KEY,1024,IPC_CREAT | 0664);
    if(shmid < 0)
    {
        perror("shmget()");
        return -1;
    }
    void* lp = shmat (shmid,NULL,0);
    while(1)
    {
        printf("%s\n",(char*)lp);
        sleep(1);
    }
    return 0;
}
