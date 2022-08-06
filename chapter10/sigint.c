#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void keycontrol(int sig)
{
    if(sig == SIGINT)
    {
        char select;
        printf("do you want to exit? (y/n)\n");
        scanf("%c", &select);
        if(select == 'y' || select == 'Y')
            exit(1);
    }
}
int main()
{
    signal(SIGINT, keycontrol);
    int i = 0;
    while(1)
    {
        printf("%d\n", i++);
        sleep(1);
    }
    return 0;
}