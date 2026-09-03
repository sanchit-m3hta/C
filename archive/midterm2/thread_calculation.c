#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 4

void* threadFunction(void* arg){
    int thread_num = *((int*)arg);
    printf("thread %d reporting: my number is %d, the square is %d\n", thread_num, thread_num, thread_num*thread_num);
    pthread_exit(NULL);
}

int main(void){
    pthread_t threads[N];
    int thread_numbers[N];
    int i;

    for (i = 0; i < N; i++){
        thread_numbers[i] = i + 1;

        if(pthread_create(&threads[i], NULL, threadFunction, (void*)&thread_numbers[i]) != 0 ){
            fprintf(stderr, "error creating thread %d\n", i + 1);
            return 1;
        }
    }
    for(i = 0; i < N; i++){
        pthread_join(threads[i], NULL);
    }


    fprintf(stdout, "all worker threads have completed\n");
    return 0;
}