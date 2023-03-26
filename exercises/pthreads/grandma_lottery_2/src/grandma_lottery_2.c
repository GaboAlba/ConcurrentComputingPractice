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
    void** retval1;
    void** retval2;
    pthread_join(thread, retval1);
    pthread_join(thread2, retval2);

    printf("MemAdress1: %p\n", &retval1);
    printf("MemAdress2: %p\n", &retval2);

    // Testing memory adresses
    __uint64_t testValue1 = *(__uint64_t*)&retval1;
    __uint64_t testValue2 = *(__uint64_t*)&retval2;
    if (testValue1 == retval1) {
        printf("Test 1 SUCCESS\n");
    }else{
        fprintf(stderr, "Test 1 FAIL\n");
    }

    if (testValue2 == retval2){
        printf("Test 2 SUCCESS\n");
    }else{
        fprintf(stderr, "Test 2 FAIL\n");
    }

    return EXIT_SUCCESS;
}//end procedure

void* grandchildren(void* data){
    __uint64_t number = (__uint64_t*)data ;
    __uint64_t* randint = (__uint64_t)rand() % 100;
    printf("I got the number: %i (%i)\n", randint, number);
    return randint;
}
