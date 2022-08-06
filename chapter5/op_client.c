#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    if (argc != 3) //�����в�����������
    {
        printf("Usage : %s <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0); //�����׽���
    if (sock == -1)                         //����ʧ��
    {
        error_handling("socket() error");
    }
    //��ʼ����ַ��Ϣ��������ַ�壬ip��ַ���˿ں�
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //��������
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }
    else
    {
        puts("connected............");
    }

    while (1)
    {
        char yn;
        int cnt, j;
        printf("input cnt:");
        scanf("%d", &cnt);
        int nums[cnt + 1];
        for (j = 0; j < cnt; j++)
        {
            printf("num %d:", j + 1);
            scanf("%d", &nums[j]);
        }
        fgetc(stdin);
        char op;
        printf("input op:");
        scanf("%c", &op);
        if (op == '+')
            nums[cnt] = 0;
        else if (op == '-')
            nums[cnt] = 1;
        else if (op == '*')
            nums[cnt] = 2;
        write(sock, nums, sizeof(nums));

        int res;
        read(sock, &res, sizeof(res));
        printf("Message from server : %d\n", res);
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
