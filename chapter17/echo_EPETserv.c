#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<errno.h>
#include<fcntl.h>
#define BUF_SIZE 5
#define EPOLL_SIZE 50

void setnonblockingmode(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    char buf[BUF_SIZE];

    if(argc != 2)
    {
        puts("argc error");
        exit(1);
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        puts("bind error");
    }
    if(listen(serv_sock, 5) == -1)
    {
        puts("listen error");
    }

    int epfd = epoll_create(EPOLL_SIZE);//创建epoll例程
    if(epfd == -1)
    {
        puts("epoll_create error");
        exit(1);
    }
    int event_cnt;//发生事件的文件描述符数
    struct epoll_event event;//发生事件的文件描述符集合
    struct epoll_event *ep_events;
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;

    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while(1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1)
        {
            puts("epoll_wait error");
            break;
        }
        puts("wait........");
        for(int i = 0; i < event_cnt; i++)//处理发生的事件
        {
            if(ep_events[i].data.fd == serv_sock)//服务套接字，即出现新的连接请求
            {
                socklen_t clnt_adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);//建立连接
                setnonblockingmode(clnt_sock);
                event.events = EPOLLIN|EPOLLET;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);//epoll客户套接字，准备收发信息
                printf("connected client %d\n", clnt_sock);
            }
            else//客户套接字，即收发信息
            {
                while (1)
                {
                    int str_len;
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE); //读取完成，关闭连接
                    if (str_len == 0)
                    {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL); //在epoll例程中删除该事件
                        close(ep_events[i].data.fd);                                //关闭套接字
                        printf("closed client %d", ep_events[i].data.fd);
                    }
                    else if(str_len < 0)//读完了缓冲区的所有数据
                    {
                        if(errno == EAGAIN)
                            break;
                    }
                    else
                    {
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                }
                
            }
        }
    }
    close(serv_sock);
    close(epfd);
}