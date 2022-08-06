#include<stdio.h>
#include<string.h>
#include<sys/uio.h>

int main()
{
    struct iovec vec[2];
    char str1[10];
    char str2[10];
    vec[0].iov_base = str1;
    vec[0].iov_len = 10;
    vec[1].iov_base = str2;
    vec[1].iov_len = 10;

    int b = readv(0, vec, 2);
    printf("\n%d bytes\n", b);
    return 0;
}