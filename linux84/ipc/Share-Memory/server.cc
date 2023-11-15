#include "com.hpp"
#include <unistd.h>


int main()
{
    Init t(SERVER);
    char* start = t.getStart();
    int n = 0;
    while(n <= 30)
    {
        cout <<"client -> server# "<< start << endl;
        sleep(1);
        n++;
    }
    return 0;
}



// int main()
// {
//     //1. 创建key
//     key_t k = getKey();
//     cout << "server key: " << toHex(k) << endl;

//     //2. 创建共享内存
//     int shmid = createShm(k, gsize);
//     cout << "server shmid: " << shmid << endl;    

//     sleep(5);

//     //3. 将自己与共享内存关联起来
//     char* start = attachShm(shmid);
//     cout<<"server 建立连接"<<endl;

//     //4. 通信
//     sleep(10);

//     //5. 断开连接
//     detachShm(start);
//     cout<<"server 断开连接"<<endl;

//     sleep(10);

//     //6. 删除共享内存
//     delShm(shmid);



// }