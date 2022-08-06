#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in recv_adr;

    if(argc != 3)
        exit(1);
    
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
    {
        puts("connect() error");
    }

    write(sock, "123", strlen("123")); 
    close(sock);
    return 0;
}