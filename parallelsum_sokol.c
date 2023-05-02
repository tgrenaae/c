#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "common.h"
#include "common_threads.h"

// TODO: include the needed libraries

volatile int array_size;
volatile int* array; // shared global array

void *mythread(void *arg) {
    int id = (int) arg;
    int start, end;
    if (id == 0) {
        start = 0;
        end = array_size / 2;
    } else {
        start = array_size / 2;
        end = array_size;
    }

    // TODO: YOUR CODE HERE
    //  IMPLEMENT THE LOGIC OF THE THREAD
    int *sum = (int *) malloc(sizeof (int));
    if (sum == NULL) {
        printf("Error: could not allocate memory.");
        exit(1);
    }

    for (int i = start; i < end; i++) {
        *sum += array[i];
    }
    return (void *) sum;

}


void print_array(int *arr, int size) {
    // assumes size > 0
    printf("array: [");
    for (int i = 0; i < size - 1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[size - 1]);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: main-first <arraysize>\n");
        exit(1);
    }
    array_size = atoi(argv[1]);
    srand(time(NULL));
    array = (int *) malloc(array_size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < array_size; i++) {
        array[i] = rand() % 100;
    }
    print_array(array, array_size);

    // TODO: YOUR CODE HERE
    //  CREATE THE TWO THREADS WITH ID 0 AND 1
    pthread_t p1, p2;
    Pthread_create(&p1, NULL, mythread, 0);
    Pthread_create(&p2, NULL, mythread, 1);

    //  MAKE THE THREAD WITH ID 0 CALCULATE AND RETURN THE SUM OF THE FIRST HALF OF THE ARRAY: STORE THE RESULT IN A VARIABLE sum1
    //  MAKE THE THREAD WITH ID 1 CALCULATE AND RETURN THE SUM OF THE SECOND HALF OF THE ARRAY: STORE THE RESULT IN A VARIABLE sum2
    int *sum1;
    int *sum2;
    Pthread_join(p1, &sum1);
    Pthread_join(p2, &sum2);

    //  CALCULATE THE TOTAL SUM AS sum = sum1 + sum2
    //  PRINT THE TOTAL SUM
    int sum = (int)*sum1 + (int)*sum2;
    printf("%d\n", sum);
    free(sum1);
    free(sum2);


    return 0;
}
