#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "common.h"
#include "common_threads.h"
#include <semaphore.h>

// TODO: include the needed libraries

// TODO: declare any necessary global variable if needed
sem_t sem2, sem3;

// TODO: implement the necessary logic so that the output of this function is the first to be printed
void *first(void *arg) {

    printf("first-");
    sem_post(&sem2);

    return NULL;
}

// TODO: implement the necessary logic so that the output of this function is the second to be printed
void *second(void *arg) {

    sem_wait(&sem2);
    printf("second-");
    sem_post(&sem3);

    return NULL;
}

// TODO: implement the necessary logic so that the output of this function is the last to be printed
void *third(void *arg) {

    sem_wait(&sem3);
    printf("third\n");
    return NULL;
}

int main(int argc, char *argv[]) {

    // TODO: YOUR CODE HERE
    //  CREATE THE THREE THREADS, WHERE EACH WILL CALL ONE OF THE THREE FUNCTIONS ABOVE
    //  WAIT FOR THE THREADS TO COMPLETE AND EXIT

    sem_init(&sem2, 0, 0);
    sem_init(&sem3, 0, 0);

    pthread_t t1, t2, t3;
    Pthread_create(&t1, NULL, first, NULL);
    Pthread_create(&t2, NULL, second, NULL);
    Pthread_create(&t3, NULL, third, NULL);
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);
    Pthread_join(t3, NULL);
    // NOTE: EVERY TIME THE CODE GETS EXECUTED THE OUTPUT SHOULD ALWAYS BE THE SAME: first-second-third

    return 0;
}
