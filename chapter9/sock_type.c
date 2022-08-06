#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

int main()
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t oplen;
    int state;

    oplen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("sock_stream = %d\n", SOCK_STREAM);
    printf("sock_dgram =  %d\n", SOCK_DGRAM);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &oplen);
    if(!state)
    {
        printf("tcp sock type: %d\n", sock_type);
    }
    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &oplen);
    if(!state)
    {
        printf("udp sock type: %d\n", sock_type);
    }
    return 0;
}