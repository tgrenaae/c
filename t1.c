#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"
#include "common_threads.h"

int max;
volatile int counter = 0; // shared global variable
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //INIT

void *mythread(void *arg) {

    char *letter = arg;
    int i; // stack (private per thread) 
    printf("%s: begin [addr of i: %p]\n", letter, &i);
    pthread_mutex_lock(&lock); //LOCK
    for (i = 0; i < max; i++) {
	    counter = counter + 1; // shared: only one
    }
    pthread_mutex_unlock(&lock); //UNLOCK

    printf("%s: done\n", letter);
    return NULL;
}
                                                                             
int main(int argc, char *argv[]) {                    
    if (argc != 2) {
	fprintf(stderr, "usage: main-first <loopcount>\n");
	exit(1);
    }
    max = atoi(argv[1]);

    pthread_t p1, p2;
    printf("main: begin [counter = %d] [%x]\n", counter, 
	   (unsigned int) &counter);
    

    Pthread_create(&p1, NULL, mythread, "A"); 
    Pthread_create(&p2, NULL, mythread, "B");


    Pthread_join(p1, NULL); 
    Pthread_join(p2, NULL); 

    printf("main: done\n [counter: %d]\n [should: %d]\n", 
	   counter, max*2);
    return 0;
}

