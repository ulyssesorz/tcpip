#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];

	if(argc != 2)//输入的参数个数有误
	{
		printf("Usage : %s <port>\n", argv[0]);
        exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		error_handling("sock() error");
	}
	//初始化地址信息
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//自动获取ip地址
	serv_addr.sin_port = htons(atoi(argv[1]));

	//将地址信息分配给套接字
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}
	//转化为可接收请求状态
	if(listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}

    fd_set reads, tmp;
    struct timeval timeout;
    int fd_max, str_len, fd_num;

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while(1)
    {
        tmp = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        fd_num = select(fd_max + 1, &tmp, 0, 0, &timeout);
        if(fd_num == -1)//出现错误
        {    
            break;
        }
        else if(fd_num == 0)//超时
        {
            //puts("timeout");
            continue;
        }
        //有套接字状态改变
        for(int i = 0; i <= fd_max; i++)//遍历存储的套接字
        {
            if(FD_ISSET(i, &tmp))//i有改变
            {
                if(i == serv_sock)//服务端有变，出现新连接
                {
                    clnt_addr_size = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                    FD_SET(clnt_sock, &reads);
                    fd_max = fd_max > clnt_sock ? fd_max : clnt_sock;
                    printf("connected client %d \n", clnt_sock);
                }
                else//客户端有改变，有信息传输
                {
                    int str_len = read(i, message, BUF_SIZE);
                    if(str_len == 0)//读完
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client %d \n", i);
                    }
                    else
                    {
                        write(i, message, str_len);
                    }
                }
            }
        }
    }
	close(serv_sock);
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
