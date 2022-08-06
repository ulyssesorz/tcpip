#include<stdio.h>
#include<unistd.h>
#define BUF_SIZE 30

int main()
{
    char msg[] = "hello";
    char msg2[] = "thank you";
    char buf[BUF_SIZE];
    int fds[2];
    int fds2[2];

    pipe(fds);
    pipe(fds2);
    int pid = fork();
    if(pid == 0)
    {
        write(fds[1], msg, sizeof(msg));
        //sleep(1);
        read(fds2[0], buf, BUF_SIZE);
        printf("child output %s\n", buf);
    }
    else
    {
        read(fds[0], buf,  BUF_SIZE);
        printf("father output %s\n", buf);
        write(fds2[1], msg2, sizeof(msg2));
        sleep(3);
    }
    return 0;
}