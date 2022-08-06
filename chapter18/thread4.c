#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#define NUM_THREAD 100

void *thread_inc(void *arg);
void *thread_des(void *arg);
long long num = 0;

int main()
{
    pthread_t pthread_id[NUM_THREAD];
    
    printf("long long = %ld\n", sizeof(long long));

    for(int i = 0; i < NUM_THREAD; i++)
    {
        if(i % 2 == 0)
        {
            pthread_create(&pthread_id[i], NULL, thread_inc, NULL);
        }
        else
        {
            pthread_create(&pthread_id[i], NULL, thread_inc, NULL);
        }
    }
    for(int i = 0; i < NUM_THREAD; i++)
    {
        pthread_join(pthread_id[i], NULL);
    }
    
    printf("res = %lld\n", num);
    return 0;
}

void *thread_inc(void *arg)
{
    for(int i = 0; i < 100000; i++)
    {
        num += 1;
    }
    return NULL;
}
void *thread_des(void *arg)
{
    for(int i = 0; i < 100000; i++)
    {
        num -= 1;
    }
    return NULL;
}