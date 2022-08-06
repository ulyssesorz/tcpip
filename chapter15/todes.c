#include<stdio.h>
#include<fcntl.h>

int main()
{
    int fd = open("data2.dat", O_WRONLY|O_CREAT|O_TRUNC);
    if(fd == -1)
    {
        puts("open error");
        return -1;
    }
    printf("fd: %d\n", fd);
    FILE *fp = fdopen(fd, "w");
    fputs("fp\n", fp);
    printf("fd: %d\n", fileno(fp));
    fclose(fp);
    return 0;
}