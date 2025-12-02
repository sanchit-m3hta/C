#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<unistd.h>
void *fun();
int shared=1; //shared variable
sem_t s; //semaphore variable


int main() {
    sem_init(&s,0,2); //initialize semaphore variable
    int num;
    printf("Enter the number of threads to be created\n");
    scanf("%d", &num);
    pthread_t thread[num];
    for(int i=0;i<num;i++){
        pthread_create(&thread[i], NULL, &fun, &i);
    }
    for(int i=0;i<num;i++){
        pthread_join(thread[i], NULL); 
    }

}
void *fun(void *x){
    int n= *(int*)x;
    sem_wait(&s); //executes wait operation on s
    printf("Thread[%d] acquired the resource\n",n);
    sleep(1); //thread1 is preempted by thread 2
    sem_post(&s);
    printf("Thread[%d] finished\n",n);
}

/*
pesudocode for counting sempahores

first create a shared variabele called "joint_account"
then createa a semaphore variable

in main():
create three threads named account1, account2, account3

*/