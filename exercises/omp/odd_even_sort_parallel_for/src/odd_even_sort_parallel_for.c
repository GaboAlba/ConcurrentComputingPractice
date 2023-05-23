#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#ifndef RAND_MAX
#define RAND_MAX ((int) ((unsigned) ~0 >> 1))
#endif


double random_double(double low, double high) {
    double randDouble;
    randDouble = (double) rand()/ ((double) RAND_MAX + 1);
    printf("%f\n", low + randDouble * (high - low));
    return (low + randDouble * (high - low));
}

void swap(double* numOne, double* numTwo) {
    double tempNumOne = *numOne;
    *numOne = *numTwo;
    *numTwo = tempNumOne;
}

double* serial_odd_even_sort(size_t n, double arr[n], size_t threadCount) {
    for (int phase = 0; phase < n; ++phase) {
        if (phase % 2 == 0) {
            #pragma omp parallel for num_threads(threadCount) \
            default(none) shared(n, arr)
            for (int i = 1; i < n; i += 2) {
                if (arr[i - 1] > arr[i]) {
                    swap(&arr[i - 1], &arr[i]);
                }
                printf("THREAD: %i\n", omp_get_thread_num());
            }
        } else {
            #pragma omp parallel for num_threads(threadCount) \
            default(none) shared(n, arr)
            for (int i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(&arr[i], &arr[i + 1]);
                }
                printf("THREAD: %i\n", omp_get_thread_num());
            }
        }
    }
    return arr;
}

void print_array(size_t n, double* arr) {
    for (size_t index = 0; index < n; index++) {
        printf("%f, ", arr[index]);
    }
    printf("\n");
}


int main(int argc, char* argv[]) {
    srand((int) time(NULL));
    size_t n = strtoull(argv[1], NULL, 10);
    size_t threadCount = strtoull(argv[2], NULL, 10);
    double* arr = calloc(n, sizeof(double));
    double* temp_arr = calloc(n, sizeof(double));
    #pragma omp parallel for num_threads(threadCount) \
          default(none) shared(n, temp_arr)
    for (size_t index = 0; index < n; index++) {
        temp_arr[index] = random_double(0.0, 100.0);
    }
    for (size_t index = 0; index < n; index++) {
        arr[index] = temp_arr[index];
    }
    printf("Unordered: ");
    print_array(n, arr);
    arr = serial_odd_even_sort(n, arr, threadCount);
    printf("Ordered: ");
    print_array(n, arr);

    free(arr);
    free(temp_arr);

    return 0;
}