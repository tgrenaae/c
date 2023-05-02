// TODO: include the needed libraries
#include <stdio.h>
#include <unistd.h>
#include "common_threads.h"
#include <semaphore.h>

// TODO: declare any necessary global variable if needed
int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

// TODO: implement the logic so that this function is the first to print
void *first() {
    pthread_mutex_lock(&mutex);
    printf("first-");
    done = 1;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// TODO: implement the logic so that this function is the second to print
void *second(void *arg) {
    pthread_mutex_lock(&mutex);
    while(done != 1){
        Pthread_cond_wait(&condition, &mutex);
    }
    printf("second-");
    done = 2;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// TODO: implement the logic so that this function is the last to print
void *third(void *arg) {
    pthread_mutex_lock(&mutex);
    while(done != 2){
        Pthread_cond_wait(&condition, &mutex);
    }
    printf("third\n");
    done = 3;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {

    // TODO: YOUR CODE HERE
    //  CREATE THE THREE THREADS, EACH WILL CALL ONE OF THE THREE FUNCTIONS ABOVE
    pthread_t t1, t2, t3;
    Pthread_create(&t1, NULL, first, NULL);
    Pthread_create(&t2, NULL, second, NULL);
    Pthread_create(&t3, NULL, third, NULL);

    //  WAIT FOR THE THREADS TO COMPLETE AND EXIT
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    // NOTE: EVERY TIME THE CODE GETS EXECUTED THE OUTPUT SHOULD ALWAYS BE THE SAME: first-second-third

    return 0;
}

