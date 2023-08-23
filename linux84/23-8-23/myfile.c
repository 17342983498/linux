#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <error.h>
#include <errno.h>



int main()
{
    const char *msg0="hello printf\n";
    const char *msg1="hello fwrite\n";
    const char *msg2="hello write\n";
    
    printf("%s", msg0);
    fwrite(msg1, strlen(msg0), 1, stdout);
    write(1, msg2, strlen(msg2));
    fork();
    return 0;
}





// int main()
// {
//     close(0);
//     int n,m;
//     int fd = open("log.txt",O_RDONLY|O_CREAT,0666);
//     scanf("%d %d",&n,&m);
//     printf("n = %d m = %d\n",n,m);
//     return 0;
// }


// int main()
// {
//     //close(1);
//     int fd = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
//     dup2(fd,1);

//     printf("hello world!\n");
//     printf("hello world!\n");
//     printf("hello world!\n");
//     printf("hello world!\n");
//     printf("hello world!\n");

//     return 0;
// }



// int main()
// {
//     umask(0);

//     close(0);
//     close(2);
//     int fd1 = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
//     int fd2 = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
//     int fd3 = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
//     int fd4 = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
//     int fd5 = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);

//     printf("%d\n",fd1);
//     printf("%d\n",fd2);
//     printf("%d\n",fd3);
//     printf("%d\n",fd4);
//     printf("%d\n",fd5);

//     return 0;
// }