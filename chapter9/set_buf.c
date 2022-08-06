#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

int main()
{
    int sock;
    int rcvbuf = 1024 * 3;
    int sndbuf = 1024 * 4;
    int state;
    socklen_t len;

    len = sizeof(rcvbuf);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcvbuf, sizeof(rcvbuf));
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&sndbuf, sizeof(sndbuf));

    len = sizeof(rcvbuf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcvbuf, &len);
    if(!state)
    {
        printf("rcvbuf size is %d\n", rcvbuf);
    }
    len = sizeof(sndbuf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&sndbuf, &len);
    if(!state)
    {
        printf("sndbuf size is %d\n", sndbuf);
    }
    return 0;
}