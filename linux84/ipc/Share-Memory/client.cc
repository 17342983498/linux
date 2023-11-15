#include "com.hpp"
#include <unistd.h>


int main()
{
    Init t(CLIENT);
    char* start = t.getStart();
    char c = 'A';

    while(c <= 'Z')
    {
        start[c - 'A'] = c;
        c++;
        start[c - 'A'] = '\0';
        sleep(1);
    }
}


// int main()
// {
//     //1. 创建key
//     key_t k = getKey();
//     cout << "server key: " << toHex(k) << endl;

//     //2. 获取共享内存
//     int shmid = getShm(k, gsize);
//     cout << "client shmid: " << shmid << endl;

//     sleep(2);

//     //3. 建立连接
//     char* start = attachShm(shmid);
//     cout<<"client 建立连接"<<endl;

//     sleep(10);

//     //4. 断开连接
//     detachShm(start);
//     cout<<"client 断开连接"<<endl;

// }