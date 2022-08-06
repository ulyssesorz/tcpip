#include<stdio.h>
#include<unistd.h>
#include<string.h>
#define BUF_SIZE 30

int main()
{
    int fds[2], fds2[2];
    pipe(fds);
    pipe(fds2);
    char str1[] = "one";
    char str2[] = "two";
    char str3[] = "three";
    char *arr[3] = {str1, str2, str3};
    char buf[BUF_SIZE];

    int pid = fork();
    if(pid == 0)
    {
        for(int i = 0; i < 3; i++)
        {
            read(fds[0], buf, BUF_SIZE);
            printf("child process output %s\n", buf);
            write(fds2[1], arr[i], strlen(arr[i]) + 1);
        }
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            write(fds[1], arr[i], strlen(arr[i]) + 1);
            read(fds2[0], buf, BUF_SIZE);
            printf("father process output %s\n", buf);
        }        
    }
    return 0;
}