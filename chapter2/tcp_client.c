#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0; //读取的字符的索引和长度

    if(argc != 3)//命令行参数个数有误
    {
        printf("Usage : %s <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);//创建套接字
    if(sock == -1)//创建失败
    {
        error_handling("socket() error");
    }
    //初始化地址信息，包括地址族，ip地址，端口号
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //请求连接
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }
    
    for(int i = 0; i < 3000; i++)//延迟一段时间
    {
        printf("Wait time %d\n", i + 1);
    }
    read(sock, message, sizeof(message));//一次读入缓冲区内所有数据
    printf("Message from server : %s\n", message);
    close(sock);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
