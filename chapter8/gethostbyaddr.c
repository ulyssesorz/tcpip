#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>

int main(int argc, char *argv[])
{
    struct hostent *host;
    struct sockaddr_in addr;
 
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
    if(!host)
        printf("host error\n");

    printf("host name: %s\n", host->h_name);
    int i;
    for(i = 0; host->h_aliases[i]; i++)
    {
        printf("alias %d: %s\n",i + 1, host->h_aliases[i]);
    }
    printf("address type: %s\n", host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
    for(i = 0; host->h_addr_list[i]; i++)
    {
        printf("ip address %d: %s\n", i + 1,
        inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    return 0;
}