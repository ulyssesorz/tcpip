#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
#define BUF_SIZE 1024
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
    char message[BUF_SIZE];
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

	if(argc != 2)//����Ĳ�����������
	{
		printf("Usage : %s <port>\n", argv[0]);
        exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		error_handling("sock() error");
	}
	//��ʼ����ַ��Ϣ
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//�Զ���ȡip��ַ
	serv_addr.sin_port = htons(atoi(argv[1]));

	//����ַ��Ϣ������׽���
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}
	//ת��Ϊ�ɽ�������״̬
	if(listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}

	while(1)
    {
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);//��������
        if(clnt_sock == -1)
        {
            close(clnt_sock);
            continue;
        }
        else
        {
            puts("new client connecting..........");
        }

        int pid = fork();
        if(pid == -1)
        {
            close(clnt_sock);
            continue;
        }
        else if(pid == 0)//�ӽ�����������
        {
            close(serv_sock);//�ӽ��̹ر��޹ص��׽���������
            int str_len;
            while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
            {
                write(clnt_sock, message, str_len);
            }
            puts("client disconnected....");
            close(clnt_sock);
            return 0;
        }
        else
        {
            close(clnt_sock);
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
void read_childproc(int sig)//��ȡ��ɾ�����ӽ�����Ϣ
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("remove proc %d\n", id);
    }
}
