//thread2.c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define N 5

void* threadFunction(void* arg){
    int thread_num = *((int*)arg);
    printf(" I am thread %d, square = %d\n", thread_num, thread_num*thread_num);
    pthread_exit(NULL);

}



int main(void){

    pthread_t threads[N];  //array of threads
    int thread_numbers[N]; //array of thread IDS
    int i;

    for(i = 0; i < N; i++){
        thread_numbers[i] = i + 1;
        if(pthread_create(&threads[i], NULL, threadFunction, (void*)&thread_numbers[i]) != 0){
            fprintf(stderr, "Error creating thread %d\n", i + 1);
            return 1;
        }
    }
    for (i = 0; i < N; i++){
        pthread_join(threads[i], NULL);
    }
    
    printf("I am the main thread\n");
    return 0;
}
/*
void* routine(int i){
    printf("I am thread %d\n", i);

}
int main(void){

    int num = 5; //number of threads

    for(int i = 0; i < num; i++){
        pthread_t t1;
        pthread_create(&t1, NULL, &routine, i);
    }
    printf("I am the main thread\n");

    return 0;
}
    */