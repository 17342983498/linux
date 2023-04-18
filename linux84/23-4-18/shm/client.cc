#include "common.hpp"

int main()
{
    key_t k =getKey();
    cout<<"client key: "<<toHex(k)<<endl;

    int shmid=getShm(k,gsize);
    cout<<"client shmid: "<<toHex(shmid)<<endl;
    return 0;
}