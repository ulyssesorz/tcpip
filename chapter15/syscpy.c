#include<stdio.h>
#include<fcntl.h>
#define BUF_SIZE 30

int main()
{
    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];

    fd1 = open("news.txt", O_RDONLY);
    fd2 = open("cpy.txt", O_WRONLY|O_CREAT|O_TRUNC);

    while((len = read(fd1, buf, BUF_SIZE)) > 0)
    {
        write(fd2, buf, len);
    }
    close(fd1);
    close(fd2);
    return 0;
}