#include "common.hpp"
#include <unistd.h>

int main()
{
    Init init(SERVER);

    // start 就已经执行了共享内存的起始空间
    char *start = init.getStart();
    int n=0;

    while(n <= 30)
    {
        cout <<"client -> server# "<< start << endl;
        sleep(1);
        n++;
    }


    //1. 创建key
/*     key_t k =getKey();
    cout<<"server key: "<<toHex(k)<<endl;

    //2.创建共享内存
    int shmid = creatShm(k,gsize);
    cout<<"server shmid: "<<toHex(shmid)<<endl;

    sleep(3);

    //3.将自己与共享内存关联起来
    char* start=attachShm(shmid);
    sleep(5);
    //4.将自己与共享内存去关联
    detachShm(start);


    sleep(5);
    //4. 删除共享内存
    delShm(shmid); */
    return 0;
}