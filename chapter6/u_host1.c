#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
    int str_len;
    char read_buf[BUF_SIZE];
    char write_buf[BUF_SIZE];

	if(argc != 2)//输入的参数个数有误
	{
		printf("Usage : %s <port>\n", argv[0]);
        exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
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
    int i;
    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        recvfrom(serv_sock, read_buf, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("received a msssage: %s\n",read_buf);

        printf("input a message you want to send: ");
        scanf("%s", write_buf);
        sendto(serv_sock, write_buf, sizeof(write_buf), 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);
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
