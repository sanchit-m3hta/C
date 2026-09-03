//DEADLOCKS.c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t first_mutex;
pthread_mutex_t second_mutex;

void* thread_one(void* arg) {
    printf("Thread ONE trying to acquire first_mutex\n");
    pthread_mutex_lock(&first_mutex);
    printf("Thread ONE acquired first_mutex\n");

    sleep(1);  // ensure T2 acquires second_mutex

    printf("Thread ONE trying to acquire second_mutex\n");
    pthread_mutex_lock(&second_mutex);
    printf("Thread ONE acquired second_mutex\n");

    pthread_mutex_unlock(&second_mutex);
    pthread_mutex_unlock(&first_mutex);
    return NULL;
}

void* thread_two(void* arg) {
    printf("Thread TWO trying to acquire second_mutex\n");
    pthread_mutex_lock(&second_mutex);
    printf("Thread TWO acquired second_mutex\n");

    sleep(1);  // ensure T1 has first_mutex

    printf("Thread TWO trying to acquire first_mutex\n");
    pthread_mutex_lock(&first_mutex);
    printf("Thread TWO acquired first_mutex\n");

    pthread_mutex_unlock(&first_mutex);
    pthread_mutex_unlock(&second_mutex);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&first_mutex, NULL);
    pthread_mutex_init(&second_mutex, NULL);

    pthread_create(&t1, NULL, thread_one, NULL);
    pthread_create(&t2, NULL, thread_two, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&first_mutex);
    pthread_mutex_destroy(&second_mutex);

    return 0;
   
}


