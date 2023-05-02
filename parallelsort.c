#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "common_threads.h"

volatile int nr_threads;
volatile int array_size;
volatile int* linear_array; // shared global array
volatile int* parallel_array; // shared global array
volatile int* sorted_parallel_array; // shared global array
volatile pthread_t* threads; // shared global array
volatile int* threads_start_index; // shared global array
volatile int* threads_end_index; // shared global array
volatile int* threads_curr_index; // shared global array


typedef struct {
    int id;
    int start;
    int end;
} myarg_t;

/**
 * Sorts arr[start, end) using insertion sort
 * @param arr
 * @param start
 * @param end
 */
void insertionSort(int *arr, int start, int end) {
    for (int step = start + 1; step < end; step++) {
        int key = arr[step];
        int j = step - 1;

        // Compare key with each element on the left of it until an element smaller than
        // it is found.
        // For descending order, change key<array[j] to key>array[j].
        while (key < arr[j] && j >= start) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void linear_sort(int *arr, int size) {
    insertionSort(arr, 0, size);
}

void *mythread(void *arg) {
    int id = (int) arg;
    int step = floor(((double)array_size / nr_threads));
    int start = id * step;
    int end = start + step;
    if (id == nr_threads - 1) {
        end = array_size;
    }
    threads_start_index[id] = start;
    threads_curr_index[id] = start;
    threads_end_index[id] = end;

    printf("%d: step=%d sorting subarray %d:%d\n", id, step, start, end - 1);
    insertionSort(parallel_array, start, end);
    printf("%d: done\n", id);
    return NULL;
}

void print_array(int *arr, int size) {
    // assumes size > 0
    printf("array: [");
    for (int i = 0; i < size - 1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[size - 1]);
}

int equal_arrays(int *arr1, int size1, int *arr2, int size2) {
    if (size1 != size2) {
        return 0;
    }
    for (int i = 0; i < size1; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: main-first <arraysize> <nrthreads>\n");
        exit(1);
    }
    array_size = atoi(argv[1]);
    nr_threads = atoi(argv[2]);
    srand(time(NULL));
    clock_t start1, end1, start2, end2;
    double cpu_time_used_linear;
    double cpu_time_used_parallel;

    linear_array = (int *) malloc(array_size * sizeof(int));
    parallel_array = (int *) malloc(array_size * sizeof(int));
    sorted_parallel_array = (int *) malloc(array_size * sizeof(int));
    threads = (pthread_t *) malloc(nr_threads * sizeof(pthread_t));
    threads_start_index = (int *) malloc(nr_threads * sizeof(int));
    threads_end_index = (int *) malloc(nr_threads * sizeof(int));
    threads_curr_index = (int *) calloc(nr_threads, sizeof(int));
    if (linear_array == NULL || parallel_array == NULL || sorted_parallel_array == NULL || threads == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < array_size; i++) {
        linear_array[i] = rand() % 1000;
        parallel_array[i] = linear_array[i];
    }
    if (array_size <= 100) {
        print_array(linear_array, array_size);
    }

    start1 = clock();
    linear_sort(linear_array, array_size);
    end1 = clock();
    cpu_time_used_linear = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
    if (array_size <= 100) {
        print_array(linear_array, array_size);
    }

    start2 = clock();
    for (int i = 0; i < nr_threads; i++) {
        Pthread_create(&threads[i], NULL, mythread, (void *) i);
    }

    // join waits for the threads to finish
    for (int i = 0; i < nr_threads; i++) {
        Pthread_join(threads[i], NULL);
    }
    if (array_size <= 100) {
        print_array(parallel_array, array_size);
    }
    // here we need to merge the sorted subarrays
    for (int i = 0; i < array_size; i++) {
//        print_array(threads_curr_index, nr_threads);
        int min = INT32_MAX;
        int min_index = -1;
        for (int j = 0; j < nr_threads; j++) {
            if (threads_curr_index[j] < threads_end_index[j] && parallel_array[threads_curr_index[j]] < min) {
                min = parallel_array[threads_curr_index[j]];
                min_index = j;
            }
        }
        sorted_parallel_array[i] = min;
        threads_curr_index[min_index]++;
    }
    end2 = clock();
    if (array_size <= 100) {
        print_array(sorted_parallel_array, array_size);
    }
    cpu_time_used_parallel = ((double) (end2 - start2)) / CLOCKS_PER_SEC;

    printf("main: done\n"
           "result: %s\n"
           "[parallel_sort] in %f seconds\n"
           "[linear_sort] in %f seconds\n",
           equal_arrays(linear_array, array_size, sorted_parallel_array, array_size) ? "Correct" : "Incorrect",
           cpu_time_used_parallel, cpu_time_used_linear);
    return 0;
}
