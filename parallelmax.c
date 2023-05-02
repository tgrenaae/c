#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "common_threads.h"

volatile int nr_threads;
volatile int array_size;
volatile int* array; // shared global array
volatile int* local_max; // shared global array
volatile pthread_t* threads; // shared global array

typedef struct {
    int id;
    int start;
    int end;
} myarg_t;

void *mythread(void *arg) {
    int id = (int) arg;
    int max = INT32_MIN;
    int step = floor(((double)array_size / nr_threads));
    int start = id * step;
    int end = start + step;
    if (id == nr_threads - 1) {
        end = array_size;
    }
    printf("%d: step=%d finding max on subarray %d:%d\n", id, step, start, end - 1);
    for (int i = start; i < end; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    local_max[id] = max;
    printf("%d: done\n", id);
    return NULL;
}

int linear_max(int *arr, int size) {
    int max = INT32_MIN;
    for (int i = 0; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
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
    if (argc != 3) {
        fprintf(stderr, "usage: main-first <arraysize> <nrthreads>\n");
        exit(1);
    }
    array_size = atoi(argv[1]);
    nr_threads = atoi(argv[2]);
    srand(time(NULL));
    clock_t start, end;
    double cpu_time_used_linear;
    double cpu_time_used_parallel;

    array = (int *) malloc(array_size * sizeof(int));
    local_max = (int *) malloc(nr_threads * sizeof(int));
    threads = (pthread_t *) malloc(nr_threads * sizeof(pthread_t));
    if (array == NULL || local_max == NULL || threads == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < array_size; i++) {
        array[i] = rand() % 1000;
    }
    if (array_size <= 100) {
        print_array(array, array_size);
    }

    start = clock();
    int max = linear_max(array, array_size);
    end = clock();
    cpu_time_used_linear = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < nr_threads; i++) {
        Pthread_create(&threads[i], NULL, mythread, (void *) i);
    }

    // join waits for the threads to finish
    for (int i = 0; i < nr_threads; i++) {
        Pthread_join(threads[i], NULL);
    }

    print_array(local_max, nr_threads);
    int parallel_max = linear_max(local_max, nr_threads);
    end = clock();
    cpu_time_used_parallel = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("main: done\n [parallel_max: %d] in %f seconds\n [linear_max: %d] in %f seconds\n",
           parallel_max, cpu_time_used_parallel, max, cpu_time_used_linear);
    return 0;
}
