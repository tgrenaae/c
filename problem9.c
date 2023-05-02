#include <stdio.h>

#include "common_threads.h"

volatile int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void* even(void* arg) {
    int *tnum = (int*) arg;
    pthread_mutex_lock(&mutex);
    printf("Hello world %d\n", *tnum);
    done++;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* odd(void* arg) {
    int *tnum = (int*) arg;
    pthread_mutex_lock(&mutex);
    while(done < 2){
        Pthread_cond_wait(&condition, &mutex);
    }
    printf("Hello world %d\n", *tnum);
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main(int argc, char *argv[]) {
    int zero = 0;
    int one = 1;
    int two = 2;
    int three = 3;
    pthread_t t0, t1, t2, t3;

    Pthread_create(&t0, NULL, even, &zero);
    Pthread_create(&t1, NULL, odd, &one);
    Pthread_create(&t2, NULL, even, &two);
    Pthread_create(&t3, NULL, odd, &three);

    Pthread_join(t0, NULL);
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);
    Pthread_join(t3, NULL);

    return 0;
}