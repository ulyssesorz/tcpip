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
    char message[BUF_SIZE];
    int str_len;

    if(argc != 3)//�����в�����������
    {
        printf("Usage : %s <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);//�����׽���
    if(sock == -1)//����ʧ��
    {
        error_handling("socket() error");
    }
    //��ʼ����ַ��Ϣ��������ַ�壬ip��ַ���˿ں�
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //��������
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }
    else
    {
        puts("connected............");
    }

    FILE *writefp = fdopen(sock, "w");
    FILE *readfp = fdopen(sock, "r");
    while(1)
    {
        if(fgets(message, sizeof(message), readfp) == NULL)
            break;
        fputs(message, stdout);
        fflush(stdout);
    }
    fputs("thank you\n", writefp);
    fflush(writefp);
    
    fclose(writefp);
    fclose(readfp);
    close(sock);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
