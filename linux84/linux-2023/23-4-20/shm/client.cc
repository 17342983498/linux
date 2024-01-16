#include "common.hpp"
#include <unistd.h>

int main()
{
    Init init(CLIENT);
    char* start=init.getStart();

    char c = 'A';
    while(c <= 'Z')
    {
        start[c - 'A'] = c;
        c++;
        start[c - 'A'] = '\0';
        sleep(1);
    }


    
/*     key_t k =getKey();
    cout<<"client key: "<<toHex(k)<<endl;

    int shmid=getShm(k,gsize);
    cout<<"client shmid: "<<toHex(shmid)<<endl;

    //3.将自己与共享内存关联起来
    char* start=attachShm(shmid);

    sleep(5);
    //4.将自己与共享内存去关联
    detachShm(start);
    sleep(5); */
    return 0;
}