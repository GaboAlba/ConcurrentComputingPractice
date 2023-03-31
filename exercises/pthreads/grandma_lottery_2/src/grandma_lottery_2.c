// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

void* grandchildren(void* data);

// procedure main:
int main(void) {
    // create_thread(greet)
    srand(time(NULL));
    pthread_t thread;
    pthread_t thread2;
    printf("Go buy some lottery\n");
    int grandchildren1 = pthread_create(&thread,
                        /*thread atributes*/ NULL,
                        grandchildren,
                        /*function arguments*/ 1);
    int grandchildren2 = pthread_create(&thread2,
                        /*thread atributes*/ NULL,
                        grandchildren,
                        /*function arguments*/ 2);
    if (grandchildren1 == EXIT_SUCCESS && grandchildren2 == EXIT_SUCCESS) {
        printf("What numbers did you get?\n");
        long int* retval1;
        long int* retval2;
        pthread_join(thread, retval1);
        pthread_join(thread2, retval2);

        printf("MemAdress1: %p value: %li\n", &retval1, retval1);
        printf("MemAdress2: %p value: %li\n", &retval2, retval2);
    } else {
        printf("At least one of the threads failed to create");
    }


    return EXIT_SUCCESS;
}
// end procedure

void* grandchildren(void* data) {
    long int number = (long int*)data;
    // Utilizing rand() as it doesn't need a seed and rand_r has been deprecated
    long int randint = (long int)rand() % 100;
    printf("I got the number: %li (%li)\n", randint, number);
    return (long int*) &randint;
}
