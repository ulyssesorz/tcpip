#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *thread_main(void *arg)
{
    int cnt = *(int*)arg;
    for(int i = 0; i < cnt; i++)
    {
        puts("running thread");
        sleep(1);
    }
}

int main()
{
    pthread_t t_id;
    int arg = 5;

    if(pthread_create(&t_id, NULL, thread_main ,(void*)&arg) != 0)
    {
        puts("error");
        return -1;
    }
    sleep(3);
    puts("end thread");
    return 0;
}