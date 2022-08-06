#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

char msg[BUF_SIZE];
char name[NAME_SIZE] = "[Anonymous]";

void *send_msg(void *arg);
void *recv_msg(void *arg);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_adr;
    pthread_t snd_thread, rcv_thread;

    if(argc != 4)
    {
        puts("arg error: <ip> <port> <name>");
        return -1;
    }

    sprintf(name, "[%s]", argv[3]);//写入姓名
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        puts("connect error");
        return -1;
    }

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);//发送信息线程
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);//接收信息线程
    pthread_join(snd_thread, NULL);
    pthread_join(rcv_thread, NULL);

    close(sock);
    return 0;
}

void *send_msg(void *arg)
{
    int sock = *(int*)arg;
    char name_msg[NAME_SIZE + BUF_SIZE];
    while(1)
    {
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(msg, "q") || !strcmp(msg, "Q"))
        {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);//合并姓名和消息
        write(sock, name_msg, strlen(name_msg));//发送给服务器
    }
    return NULL;
}
void *recv_msg(void *arg)
{
    int sock = *(int*)arg;
    char name_msg[NAME_SIZE + BUF_SIZE];
    while(1)
    {
        int str_len;
        str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);//接收信息
        if(str_len == -1)
        {
            return (void*)-1;
        }
        name_msg[str_len] = 0;
        printf("%s\n", name_msg);
    }
}