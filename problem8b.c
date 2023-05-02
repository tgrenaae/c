//libraries needed
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "common.h"
#include "common_threads.h"
#include <semaphore.h>

//global variables
int a[10]  = {4,3,7,-2,4,-1,6,-3,-8,0};
sem_t sem1;


void *thread0(){
    for(int i = 0; i < 10; i++){
        if(a[i] >= 0){
            printf("0 %d \n", a[i]);
        }
    }
    sem_post(&sem1);
    return NULL;
}

void *thread1(){
    sem_wait(&sem1);
    for(int i = 0; i < 10; i++){
        if(a[i] < 0){
            printf("1 %d \n", a[i]);
        }
    }
    sem_post(&sem1);
    return NULL;
}


int main(void){
    sem_init(&sem1, 0, 0);
    pthread_t t0, t1;
    Pthread_create(&t0, NULL, thread0, NULL);
    Pthread_create(&t1, NULL, thread1, NULL);
    Pthread_join(t0, NULL);
    Pthread_join(t1, NULL);
    return 0;
}
