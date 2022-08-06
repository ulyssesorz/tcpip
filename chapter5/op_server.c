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

	clnt_addr_size = sizeof(clnt_addr);
    int str_len;
    int i;
	for(i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);//��������
        if(clnt_sock == -1)
        {
            error_handling("accept() error");
        }
        else
        {
            printf("connecting client %d", i + 1);
        }
        
        int nums[BUF_SIZE];
        while(1)
        {
        int len = read(clnt_sock, nums, BUF_SIZE) / sizeof(int);
        int res = nums[0];
        int j;
        for(j = 1; j < len - 1; j++)
        {
            if(nums[len - 1] == 0)
                res += nums[j];
            else if(nums[len - 1] == 1)
                res -= nums[j];
            else if(nums[len - 1] == 2)
                res *= nums[j];
        }
        write(clnt_sock, &res, sizeof(res));
        }
        close(clnt_sock);
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
