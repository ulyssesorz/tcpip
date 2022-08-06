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
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, 0);
    alarm(2);

    for(int i = 0; i < 3; i++)
    {
        puts("wait..\n");
        sleep(1000);
    }
}