#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void* grandchildren(void* data);

//procedure main:
int main(void){
    //create_thread(greet)
    srand(time(NULL));
    pthread_t thread;
    pthread_t thread2;
    printf("Go buy some lottery\n");
    int grandchildren1 = pthread_create(&thread,/*thread atributes*/ NULL, grandchildren, /*function arguments*/ 1);
    int grandchildren2 = pthread_create(&thread2,/*thread atributes*/ NULL, grandchildren, /*function arguments*/ 2);
    printf("What numbers did you get?\n");
    char** retval1;
    char** retval2;
    pthread_join(thread, retval1);
    pthread_join(thread2, retval2);

    printf("MemAdress1: %c\n", &retval1);
    printf("MemAdress2: %c\n", &retval2);

    return EXIT_SUCCESS;
}//end procedure

void* grandchildren(void* data){
    int number = data ;
    int* randint = rand() % 100;
    printf("I got the number: %i (%i)\n", randint, number);
    return randint;
}
