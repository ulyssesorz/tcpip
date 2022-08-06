#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#define NUM_THREAD 100

void *get_num(void *arg);
void *cal_num(void *arg);
static int num = 0;
static sem_t sem_one;
static sem_t sem_two;

int main()
{
    pthread_t id1, id2;
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    pthread_create(&id1, NULL, get_num, NULL);
    pthread_create(&id2, NULL, cal_num, NULL);
    
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}

void *get_num(void *arg)
{
    for(int i = 0; i < 5; i++)
    {
        puts("input a number:");
        sem_wait(&sem_two);
        scanf("%d", &num);
        sem_post(&sem_one);
    }
    return NULL;
}
void *cal_num(void *arg)
{
    int sum = 0;
    
    for(int i = 0; i < 5; i++)
    {
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }

    printf("res = %d\n", sum);
    return NULL;
}3