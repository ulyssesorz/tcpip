#include<stdio.h>
#include<unistd.h>

int main()
{   
    pid_t pid;

    pid = fork();
    if(pid == 0)
    {
        printf("i'm child process\n");
    }
    else
    {
        sleep(30);
        printf("i'm father process, child is %d\n", pid);
    }
    if(pid == 0)
        printf("end child process\n");
    else
        printf("end father process\n");
}