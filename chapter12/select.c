#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>
#define BUF_SIZE 30

int main()
{
    fd_set reads, tmp;
    int res, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while(1)
    {
        tmp = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        res = select(1, &tmp, 0, 0, &timeout);
        if(res == -1)
        {
            puts("error");
            break;
        }
        else if(res == 0)
        {
            puts("time out");
        }
        else
        {
            if(FD_ISSET(0, &tmp))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message: %s\n", buf);
            }

        }
    }
    return 0;
}