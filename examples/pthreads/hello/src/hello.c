#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* greet(void* data);

//procedure main:
int main(void){
    //create_thread(greet)
    pthread_t thread;
    int error = pthread_create(&thread,/*thread atributes*/ NULL, greet, /*function arguments*/ NULL);
    if(error == EXIT_SUCCESS){
        //print("Hello from main thread")
        printf("Hello from main thread\n");
        pthread_join(thread, NULL);
    }else {
        fprintf(stderr, "Error: could not create secondary thread");
    }
    return error;
}//end procedure

//procedure greet:
void* greet (void* data){
    (void*) data;
    //print("Hello from secondary thread")
    printf("Hello from secondary thread\n");
    for(int x = 0; x < 1000000; ++x){
        printf("Saludo %d de el hilo 2\n", x);
    }
    return NULL;
} //end procedure
    
