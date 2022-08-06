#include<stdio.h>
#include<unistd.h>
#define BUF_SIZE 30

int main()
{
    char msg[] = "hello";
    char buf[BUF_SIZE];
    int fds[2];

    pipe(fds);
    int pid = fork();
    if(pid == 0)
    {
        write(fds[1], msg, sizeof(msg));
    }
    else
    {
        read(fds[0], buf,  BUF_SIZE);
        puts(buf);
    }
    return 0;
}