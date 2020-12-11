#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        perror("socket()");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(19999);
    addr.sin_addr.s_addr = inet_addr("192.168.80.134");
    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind()");
        return -1;
    }

    ret = listen(sockfd,5);
    if(ret < 0)
    {
        perror("listen()");
        return -1;
    }

    int newsockfd = accept(sockfd,NULL,NULL);
    if(newsockfd < 0)
    {
        perror("accept()");
        return -1;
    }
    
    while(1)
    {
        char buf[1024]={0};
        ret = recv(newsockfd,buf,sizeof(buf),0);
        if(ret < 0)
        {
            perror("ret()");
            return -1;
        }
        else if(ret == 0)
        {
            printf("peer shutdown!\n");
            close(newsockfd);
            return 0;
        }
        printf("cli say:%s\n",buf);
        memset(buf,'\0',sizeof(buf));
        const char* str = "i am server!";
        sleep(1);
        strncpy(buf,str,strlen(str));
        ret = send(newsockfd,buf,strlen(buf),0);
        if(ret < 0)
        {
            perror("send()");
            return -1;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
