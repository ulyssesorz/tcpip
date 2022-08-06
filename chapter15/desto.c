#include<stdio.h>
#include<fcntl.h>

int main()
{
    int fd = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
    if(fd == -1)
    {
        puts("open error");
        return -1;
    }
    FILE *fp = fdopen(fd, "w");
    fputs("network c programming", fp);
    fclose(fp);
    return 0;
}