#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0)
    {
        perror("pipe()");
        return -1;
    }

    int flag = fcntl(fd[1],F_GETFL);
    fcntl(fd[1],F_SETFL,flag | O_NONBLOCK);

    int pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        return -1;
    }
    else if(pid == 0)
    {
        //子进程：写
        close(fd[0]);
        int count = 0;
        while(1)
        {
            ssize_t r_size = write(fd[1],"h",1);
            if(r_size < 0)
            {
                perror("write()");
                printf("r_size : %d\n",r_size);
                break;
            }
            count++;
            printf("count : %d\n",count);
        }
    }
    else
    {
        //父进程
        //close(fd[0]);
        while(1)
        {
            sleep(1);
        }
    }
    return 0;
}
