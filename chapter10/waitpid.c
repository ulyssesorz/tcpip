#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
    pid_t pid = fork();
    int status;

    if(pid == 0)
    {
        sleep(3);
        exit(10);
    }
    else
    {
        while(!waitpid(pid, &status, WNOHANG))
        {
            sleep(1);
            printf("sleep 1sec\n");
        }
        if(WIFEXITED(status))
        {
            printf("child process send %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}