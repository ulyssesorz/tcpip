#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>

int main()
{
    int fd1, fd2, fd3;
    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("text.dat", O_CREAT | O_WRONLY | O_TRUNC);
    fd3 = socket(PF_INET, SOCK_STREAM, 0);

    printf("file descriptor1: %d\n", fd1);
    printf("file descriptor2: %d\n", fd2);
    printf("file descriptor3: %d\n", fd3);

    return 0;
}
