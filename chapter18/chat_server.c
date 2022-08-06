#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

int clnt_socks[MAX_CLNT];
int clnt_num = 0;
pthread_mutex_t mutex;

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);

int main(int argc, char *argv[])
{
    int serv_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    pthread_t t_id;

    if(argc != 2)
    {
        puts("arg error: <port>");
        return -1;
    }

    pthread_mutex_init(&mutex, NULL);//初始化互斥量
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        puts("bind error");
        return -1;
    }
    if(listen(serv_sock, 5) == -1)
    {
        puts("listen error");
        return -1;
    }

    while(1)
    {
        socklen_t clnt_adr_sz = sizeof(clnt_adr);
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
        {
            puts("accept error");
            return -1;
        }

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_num++] = clnt_sock;//把新连接加入数组
        pthread_mutex_unlock(&mutex);//后续线程还要操作数组，所以要上锁

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
        printf("conneced client ip: %s\n", inet_ntoa(clnt_adr.sin_addr));
    }
    close(serv_sock);
    return 0;
}

void *handle_clnt(void *arg)
{
    int clnt_sock = *(int*)arg;
    int str_len;
    char buf[BUF_SIZE];
    while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)//读取信息并发送
    {
        send_msg(buf, str_len);
    }
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < clnt_num; i++)
    {
        if(clnt_socks[i] == clnt_sock)
        {
            while(i++ < clnt_num - 1)//i之后的每个元素前移一位，删除i
            {
                clnt_socks[i] = clnt_socks[i + 1];
            }
            break;
        }
    }
    clnt_num--;
    pthread_mutex_unlock(&mutex);

    close(clnt_sock);
    return NULL;
}
void send_msg(char *msg, int len)
{
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < clnt_num; i++)//遍历clnt_socks，向每个客户发送信息
    {
        write(clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}