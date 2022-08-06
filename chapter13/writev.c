#include<stdio.h>
#include<string.h>
#include<sys/uio.h>

int main()
{
    struct iovec vec[2];
    char str1[] = "hello";
    char str2[] = "world";
    vec[0].iov_base = str1;
    vec[0].iov_len = strlen(str1);
    vec[1].iov_base = str2;
    vec[1].iov_len = strlen(str2);

    int b = writev(1, vec, 2);
    printf("\n%d bytes\n", b);
    return 0;
}