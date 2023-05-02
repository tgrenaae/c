#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include "common.h"
#include "common_threads.h"

sem_t a,b,c,d;

int value = 0;

void *critical_1(void *arg){
    sem_wait(&a);
    sem_wait(&b);
    sem_wait(&c);
    value = value + 1;
    sem_post(&c);
    sem_post(&b);
    sem_post(&a);
}

void *critical_2(void *arg){
    sem_wait(&b);
    sem_wait(&c);
    sem_wait(&d);
    value = value + 1;
    sem_post(&d);
    sem_post(&c);
    sem_post(&b);
}

void *critical_3(void *arg){
    sem_wait(&a);
    sem_wait(&c);
    sem_wait(&d);
    value = value + 1;
    sem_post(&d);
    sem_post(&c);
    sem_post(&a);
}

int main(){

    pthread_t t1,t2,t3;

    //Initilizing the semaphore
    sem_init(&a, 0, 1); 
    sem_init(&b, 0, 1); 
    sem_init(&c, 0, 1); 
    sem_init(&d, 0, 1);

    //Createing the threads and calling the critical section
    Pthread_create(&t1,NULL,critical_1,NULL);
    Pthread_create(&t2,NULL,critical_2,NULL);
    Pthread_create(&t3,NULL,critical_3,NULL);


    Pthread_join(t1,NULL);
    Pthread_join(t2,NULL);
    Pthread_join(t3,NULL);

    printf("Value: %d",value);

    return 0;
}