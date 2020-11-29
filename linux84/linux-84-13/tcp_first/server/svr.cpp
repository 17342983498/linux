#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0)
    {
        perror("socked\n");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(19999);
    addr.sin_addr.s_addr = inet_addr("192.168.80.133");
    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret<0)
    {
        perror("bind\n");
        return -1;
    }

    ret = listen(sockfd,5);
    if(ret<0)
    {
        perror("listen\n");
        return -1;
    }

    int newsockfd = accept(sockfd,NULL,NULL);
    if(newsockfd<0)
    {
        perror("accept\n");
        return -1;
    }

    while(1)
    {
        char buf[1024]={0};
        ret = recv(newsockfd,buf,sizeof(buf)-1,0);
        if(ret<0)
        {
            perror("recv\n");
            return -1;
        }
        else if(ret==0)
        {
            printf("peer shutdown\n");
            close(newsockfd);
            return 0;
        }
        printf("cli say: %s\n",buf);
        memset(buf,'\0',sizeof(buf));
        const char* str = "i am server: ";
        strncpy(buf,str,strlen(str));
        ret = send(newsockfd,buf,strlen(buf),0);
        if(ret<0)
        {
            perror("send\n");
            return -1;
        }
    }
    close(sockfd);
    close(newsockfd);
    return 0;
}
