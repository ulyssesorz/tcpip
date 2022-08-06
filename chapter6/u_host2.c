#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char read_buf[BUF_SIZE];
    char write_buf[BUF_SIZE];
    int str_len;
    socklen_t serv_addr_size;

    if(argc != 3)//�����в�����������
    {
        printf("Usage : %s <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);//�����׽���
    if(sock == -1)//����ʧ��
    {
        error_handling("socket() error");
    }
    //��ʼ����ַ��Ϣ��������ַ�壬ip��ַ���˿ں�
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    while(1)
    {
        serv_addr_size = sizeof(serv_addr);
        printf("input a message you want to send: ");
        scanf("%s", write_buf);
        sendto(sock, write_buf, sizeof(write_buf), 0, (struct sockaddr*)&serv_addr, serv_addr_size);
        
        recvfrom(sock, read_buf, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_size);
        printf("received a msssage: %s\n",read_buf);
    }
    
    close(sock);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
