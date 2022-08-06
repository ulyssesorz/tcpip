#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

void *thread_main(void *arg)
{
    int cnt = *(int*)arg;
    for(int i = 0; i < cnt; i++)
    {
        puts("running thread");
        sleep(1);
    }
    char *msg = (char*)malloc(sizeof(char) * 10);
    strcpy(msg, "hello!");
    return (void*)msg;
}

int main()
{
    pthread_t t_id;
    int arg = 5;
    void *ret;

    if(pthread_create(&t_id, NULL, thread_main ,(void*)&arg) != 0)
    {
        puts("error");
        return -1;
    }
    if(pthread_join(t_id, &ret) != 0)
    {
        puts("join error");
        return -1;
    }
    printf("ret = %s\n", (char*)ret);
    free(ret);
    return 0;
}