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
	int recv_sock;
	struct sockaddr_in adr;
	struct ip_mreq join_adr;
    char message[BUF_SIZE];

	if(argc != 3)//����Ĳ�����������
	{
		printf("Usage : %s <group id> <port>\n", argv[0]);
        exit(1);
	}

	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(recv_sock == -1)
	{
		error_handling("sock() error");
	}
	//��ʼ����ַ��Ϣ
	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);//�Զ���ȡip��ַ
	adr.sin_port = htons(atoi(argv[2]));

	//����ַ��Ϣ������׽���
	if(bind(recv_sock, (struct sockaddr*) &adr, sizeof(adr)) == -1)
	{
		error_handling("bind() error");
	}
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

	while (1)
    {
        int str_len;
        str_len = recvfrom(recv_sock, message, BUF_SIZE - 1, 0, NULL, 0);
        if(str_len < 0)
            break;
        message[str_len] = 0;
        printf("%s", message);
    }
    
	close(recv_sock);
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
