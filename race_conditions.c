#include <stdio.h>
#include <pthread.h>

long count = 0;

void *thread_increment(void *arg)
{
    int i;

    i = 0;
    while(i < 1000000)
    {
        count++;
        i++;
    }

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_increment, NULL);
    pthread_create(&t2, NULL, thread_increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %ld\n", count);
    printf("Expected value: 2000000\n");

    return 0;
}
