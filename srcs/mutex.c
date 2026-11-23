#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock;   // 1. Declare the "key"

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);     // 2. Take the key
        counter++;                    // 3. Do the risky part
        pthread_mutex_unlock(&lock);  // 4. Return the key
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);  // Set up the key/lock before use

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter: %d\n", counter);  // Now always 200000

    pthread_mutex_destroy(&lock);  // Clean up the lock when totally done
    return 0;
}
