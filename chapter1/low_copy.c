//buf后出现后乱码，已解决
//还有文件权限问题

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#define BUF_SIZE 100
void error_handling(char *message);

int main()
{
    int fd1, fd2;
    char buf[BUF_SIZE];

    fd1 = open("data.txt", O_RDONLY);
    fd2 = open("data2.txt", O_CREAT | O_WRONLY);

    if(fd1 == -1 || fd2 == -1)
    {
        error_handling("open() error");
    }
    int cnt = read(fd1, buf, sizeof(buf));//返回的字节数
    if(cnt == -1)
    {
        error_handling("read() error");
    }
    if((write(fd2, buf, cnt)) == -1)
    {
        error_handling("write() error");
    }
    printf("cnt = %d\n", cnt);
    printf("fd2 = %d\n", fd2);
    
    close(fd1);
    close(fd2);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
