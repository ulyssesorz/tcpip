#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message1[] = "hello";
    char message2[] = "world";
    char message3[] = "!!!";
    int str_len;

    if(argc != 3)//命令行参数个数有误
    {
        printf("Usage : %s <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);//创建套接字
    if(sock == -1)//创建失败
    {
        error_handling("socket() error");
    }
    //初始化地址信息，包括地址族，ip地址，端口号
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));


        
    socklen_t serv_addr_size = sizeof(serv_addr);
    sendto(sock, message1, sizeof(message1), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
    sendto(sock, message2, sizeof(message2), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
    sendto(sock, message3, sizeof(message3), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
    
    close(sock);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
