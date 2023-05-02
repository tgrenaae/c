#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "common_threads.h"

// TODO: include the needed libraries

volatile int array_size;
volatile int* array; // shared global array
volatile int sum = 0;

void *mythread(void *arg) {
    
    int id = (int) arg;
    int a_half = floor(array_size/2);

    
    if(id == 1){
        for(int i = 0; i < a_half; i++){
            sum += array[i];
        }
        
    }else{
        for(int i = a_half; i < array_size; i++){
            sum += array[i];
        }
    }
    // TODO: YOUR CODE HERE
    //  IMPLEMENT THE LOGIC OF THE THREAD
    return
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "usage: main-first <arraysize>\n");
        exit(1);
    }
    printf("arrgv: %s", argv[1]);
    array_size = atoi(argv[1]);
    
    srand(time(NULL));

    array = (int *) malloc(array_size * sizeof(int));
    
    if (array == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(1);
    }
    printf("array:");
    for (int i = 0; i < array_size; i++) {
        array[i] = rand() % 100;
        printf("%d, ",array[i]);
    }
    printf("\n");




    pthread_t t0,t1;

    Pthread_create(&t0,NULL,mythread,(void *) 0);
    Pthread_create(&t1,NULL,mythread,(void *) 1);
    
    Pthread_join(t0,NULL);
    Pthread_join(t1,NULL);

    printf("Sum: %d",sum);


    // TODO: YOUR CODE HERE
    //  CREATE THE TWO THREADS WITH ID 0 AND 1
    //  MAKE THE THREAD WITH ID 0 CALCULATE AND RETURN THE SUM OF THE FIRST HALF OF THE ARRAY: STORE THE RESULT IN A VARIABLE sum1
    //  MAKE THE THREAD WITH ID 1 CALCULATE AND RETURN THE SUM OF THE SECOND HALF OF THE ARRAY: STORE THE RESULT IN A VARIABLE sum2
    //  CALCULATE THE TOTAL SUM AS sum = sum1 + sum2
    //  PRINT THE TOTAL SUM

    return 0;
}
