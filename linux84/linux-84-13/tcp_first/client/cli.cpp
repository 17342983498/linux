#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(19999);
    addr.sin_addr.s_addr = inet_addr("192.168.80.133"); 
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return -1;
    }

    while(1)
    {
        char buf[1024]={0};
        strcpy(buf,"i am cli");
        sleep(1);
        ret = send(sockfd,buf,strlen(buf),0);
        if(ret < 0)
        {
            perror("send");
            return -1;
        }
        memset(buf,'\0',sizeof(buf)-1);
        ret = recv(sockfd,buf,sizeof(buf)-1,0);
        if(ret<0)
        {
            perror("recv");
            return -1;
        }
        else if (ret == 0)
        {
            printf("peer shutdown\n");
            close(sockfd);
            return -1;
        }
        printf("svr say:%s\n",buf);
    }
    return 0;
}
