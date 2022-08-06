#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#define BUF_SIZE 30

int acpt_sock, recv_sock;

void urg_handler(int sig)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    printf("urgent message: %s\n", buf);
}

int main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    struct sockaddr_in recv_adr, serv_adr;
    socklen_t serv_adr_sz;
    struct sigaction act;

    if(argc != 2)
        exit(1);
    
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

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

    fcntl(recv_sock, F_SETOWN, getpid());
    int state = sigaction(SIGURG, &act, 0);

    int str_len;
    while((str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0)) != 0)
    {
        if(str_len == -1)
            continue;
        buf[str_len] = 0;
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;
}