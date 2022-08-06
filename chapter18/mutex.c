#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#define NUM_THREAD 100

void *thread_inc(void *arg);
void *thread_des(void *arg);
long long num = 0;
pthread_mutex_t mutex;

int main()
{
    pthread_t pthread_id[NUM_THREAD];
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < NUM_THREAD; i++)
    {
        if(i % 2 == 0)
        {
            pthread_create(&pthread_id[i], NULL, thread_inc, NULL);
        }
        else
        {
            pthread_create(&pthread_id[i], NULL, thread_des, NULL);
        }
    }
    for(int i = 0; i < NUM_THREAD; i++)
    {
        pthread_join(pthread_id[i], NULL);
    }
    
    printf("res = %lld\n", num);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void *thread_inc(void *arg)
{
    pthread_mutex_lock(&mutex);//在for外面上锁，否则每个线程上锁100000次
    for(int i = 0; i < 100000; i++)
    {
        num += 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *thread_des(void *arg)
{
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < 100000; i++)
    {
        num -= 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}