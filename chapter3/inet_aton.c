//ip×Ö·û´®×ªÕûĞÍ
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main()
{
    char *addr = "1.2.3.4";
    struct sockaddr_in addr_inet;

    if(inet_aton(addr, &addr_inet.sin_addr))
    {
        printf("addr = %#x\n", addr_inet.sin_addr.s_addr);
    }
    else
    {
        printf("error\n");
    }
    return 0;
}