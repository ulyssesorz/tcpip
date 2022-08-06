#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("time out\n");
    alarm(2);
}
void keycontrol(int sig)
{
    if(sig == SIGINT)
        puts("ctrl c\n");
}
int main()
{
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for(int i = 0; i < 3; i++)
    {
        puts("wait..\n");
        sleep(1000);
    }
}