#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg)
{
    pthread_mutex_lock(&mutex_a);
    printf("Thread 1 locked A\n");

    sleep(1);

    pthread_mutex_lock(&mutex_b);
    printf("Thread 1 locked B\n");

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);

    return NULL;
}

void *thread2(void *arg)
{
    pthread_mutex_lock(&mutex_b);
    printf("Thread 2 locked B\n");

    sleep(1);

    pthread_mutex_lock(&mutex_a);
    printf("Thread 2 locked A\n");

    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);

    return NULL;
}

int main(void)
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
