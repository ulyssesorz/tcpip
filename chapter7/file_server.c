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

	char message[BUF_SIZE];
	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		error_handling("sock() error");
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}
	if(listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size
			);
	if(clnt_sock == -1)
	{
		error_handling("accept() error");
	}
    char filename[] = "data1.dat";
	FILE *fp;
	fp = fopen(filename, "rb");//�Զ���ʽ���ļ�
	int read_cnt;
	while(1)
	{
		read_cnt = fread((void*)message, 1, BUF_SIZE, fp);//��ȡ�ļ��ݴ浽������message��
		if(read_cnt < BUF_SIZE)//����С�ڻ�������С��һ���Զ���
		{
			write(clnt_sock, message, read_cnt);
			break;
		}
		write(clnt_sock, message, BUF_SIZE);//ÿ�ζ�ȡ��������С������
	}
	
	shutdown(clnt_sock, SHUT_WR);
	read(clnt_sock, message, BUF_SIZE);
	printf("message from client: %s\n", message);

	fclose(fp);
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
