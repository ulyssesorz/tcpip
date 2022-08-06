#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int acpt_sock, recv_sock;
    char buf[BUF_SIZE];
    struct sockaddr_in recv_adr, serv_adr;
    socklen_t serv_adr_sz;

    if(argc != 2)
        exit(1);

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
    {
        puts("bind() error");
    }
    listen(acpt_sock, 5);

    serv_adr_sz = sizeof(serv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

    int str_len;
    while(1)
    {
        str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK|MSG_DONTWAIT);
        if(str_len > 0)
            break;
    }
    buf[str_len] = 0;
    printf("buf: %s\n", buf);

    str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[BUF_SIZE] = 0;
    printf("read again: %s\n", buf);
    close(recv_sock);
    close(acpt_sock);
    return 0;
}