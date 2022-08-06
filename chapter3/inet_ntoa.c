//ip整型转字符串
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

int main()
{
    struct sockaddr_in addr1, addr2;
    char *str1;
    char str2[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    str1 = inet_ntoa(addr1.sin_addr);
    strcpy(str2, str1);
    printf("%s\n", str1);

    inet_ntoa(addr2.sin_addr);//覆盖了str1
    printf("%s\n", str1);
    printf("%s\n", str2);
    return 0;
}