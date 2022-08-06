#include<stdio.h>
#include<unistd.h>

int gval = 10;
int main()
{   
    pid_t pid;
    int lval = 20;
    lval = 20;

    pid = fork();
    if(pid == 0)
    {
        gval++;
        lval++;
    }
    else
    {
        gval--;
        lval--;
    }
    if(pid == 0)
        printf("child process %d %d\n", gval, lval);
    else
        printf("father process %d %d\n", gval, lval);
}