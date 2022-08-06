#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
void error_handling(char *message);

int main()
{
    int fd;
    char buf[] = "Let's Go\n";//要写入的数据

    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);//新建，读写，清空原文件
    if(fd == -1)//打开失败
    {
        error_handling("open() error");
    }
    printf("file descriptor: %d\n", fd);
    if((write(fd, buf, sizeof(buf))) == -1)//缓冲区向文件写入数据
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
