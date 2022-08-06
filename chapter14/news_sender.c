#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
#define TTL 64
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int send_sock;
    struct sockaddr_in mul_adr;
    char message[BUF_SIZE];
    int time_live = TTL;

    if(argc != 3)//�����в�����������
    {
        printf("Usage : %s <group id> <port>\n", argv[0]);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);//�����׽���
    if(send_sock == -1)//����ʧ��
    {
        error_handling("socket() error");
    }
    //��ʼ����ַ��Ϣ��������ַ�壬ip��ַ���˿ں�
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
    FILE *fp = fopen("news.txt", "r");
    if(fp == NULL)
        error_handling("fopen() error");

    while(!feof(fp))
    {
        fgets(message, BUF_SIZE, fp);
        sendto(send_sock, message, strlen(message), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
