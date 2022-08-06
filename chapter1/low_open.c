#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
void error_handling(char *message);

int main()
{
    int fd;
    char buf[] = "Let's Go\n";//Ҫд�������

    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);//�½�����д�����ԭ�ļ�
    if(fd == -1)//��ʧ��
    {
        error_handling("open() error");
    }
    printf("file descriptor: %d\n", fd);
    if((write(fd, buf, sizeof(buf))) == -1)//���������ļ�д������
    {
        error_handling("write() error");
    }
    close(fd);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
