#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

void *thread_main(void *arg)
{
    int *res = (int*)malloc(sizeof(int));
    int beg = *(int*)arg;
    for(int i = beg; i < beg + 5; i++)
    {
        *res += i;
    }
    return (void*)res;
}

int main()
{
    pthread_t t_id;
    pthread_t t_id2;
    int beg = 1;
    int beg2 = 6;
    void *ret;
    void *ret2;

    if(pthread_create(&t_id, NULL, thread_main ,(void*)&beg) != 0)
    {
        puts("create error");
        return -1;
    }
    if(pthread_create(&t_id2, NULL, thread_main ,(void*)&beg2) != 0)
    {
        puts("create error");
        return -1;
    }
    if(pthread_join(t_id, &ret) != 0)
    {
        puts("join error");
        return -1;
    }
    if(pthread_join(t_id2, &ret2) != 0)
    {
        puts("join error");
        return -1;
    }
    int sum = *(int*)ret + *(int*)ret2;
    printf("res = %d\n", sum);
    return 0;
}