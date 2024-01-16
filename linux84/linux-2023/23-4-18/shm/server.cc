#include "common.hpp"
#include <unistd.h>

int main()
{
    //1. 创建key
    key_t k =getKey();
    cout<<"server key: "<<toHex(k)<<endl;

    //2.创建共享内存
    int shmid=creatShm(k,gsize);
    cout<<"server shmid: "<<toHex(shmid)<<endl;

    sleep(5);

    //3. 删除共享内存
    delShm(shmid);
    return 0;
}