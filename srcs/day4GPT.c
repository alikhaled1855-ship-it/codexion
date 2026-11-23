#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int ready = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *worker(void *arg)
{
    pthread_mutex_lock(&mutex);

    while (ready == 0)
        pthread_cond_wait(&cond, &mutex);

    printf("Worker: data is ready!\n");

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *controller(void *arg)
{
    sleep(1);

    pthread_mutex_lock(&mutex);

    ready = 1;

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, controller, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
