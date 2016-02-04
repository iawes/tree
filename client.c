/*===============================================================
*   文件名称：client.c
*   创建日期：2016年02月02日
================================================================*/
//#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 4096

int main(int argc, char **argv)
{
    int sockfd, n, rec_len;
    char recvline[4096] = {0};
    char sendline[4096] = {0};
    char buf[MAXLINE];
    struct sockaddr_in servaddr;

    if(argc != 2)
    {
        printf("usage: ./client <ipaddress>\n");
        exit(0);
    }

    if(0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("create socket error: %s(error: %d)\n", strerror(errno), errno);
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    if(0 >= inet_pton(AF_INET, argv[1], &servaddr.sin_addr))
    {
        printf("inet_pton error for %s\n",  argv[1]);
        exit(0);
    }

    if(0 > connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        printf("connect socket error: %s(error: %d)\n", strerror(errno), errno);
        exit(0);
    }

    printf("send msg to server:\n");

    fgets(sendline, 4096, stdin);

    if(0 > send(sockfd, sendline, strlen(sendline), 0))
    {
        printf("sendsocket error: %s(error: %d)\n", strerror(errno), errno);
        exit(0);
    }

    if(-1 == (rec_len = recv(sockfd, buf, MAXLINE, 0)))
    {
        perror("recv error");
        exit(0);
    }

    buf[rec_len] = '\0';
    printf("Received : %s", buf);
    close(sockfd);

    return 0;
}