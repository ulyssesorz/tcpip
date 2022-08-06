#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

void read_childproc(int sig)//读取待删除的子进程信息
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("remove proc %d\n", id);
        printf("child process send %d\n", WEXITSTATUS(status));
    }
}
int main()
{
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    pid_t pid = fork();

    if(pid == 0)
    {
        puts("i'm child process");
        sleep(10);
        exit(10);
    }
    else
    {
        printf("child process id: %d\n", pid);
        pid = fork();
        if(pid == 0)
        {
            puts("i'm child process");            
            sleep(10);
            exit(20);
        }
        else
        {
            printf("child process id: %d\n", pid);
            for(int i = 0; i < 5; i++)
            {
                puts("wait......");
                sleep(5);
            }
        }

    }
    return 0;
}