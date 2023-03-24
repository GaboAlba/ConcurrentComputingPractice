#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* greet(void* data);

//procedure main:
int main(void){
    //create_thread(greet)
    pthread_t thread;
    int size_t = 8;
    int error = pthread_create(&thread,/*thread atributes*/ NULL, greet, /*function arguments*/ size_t);
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
    int size_t = data;
    pthread_t* thread = (pthread_t*) malloc(size_t * sizeof(pthread_t));
    // Procedure if size_t is 0
    if (size_t == 0) {
        printf("Hasta luego desde: %i \n", size_t);
    }else {
        //printf("Saludo desde: %i \n", size_t);
        if (size_t == 1){
            size_t--;
            int error = pthread_create(&thread[size_t],/*thread atributes*/ NULL, greet, /*function arguments*/ size_t);
            //pthread_join(thread[size_t], NULL);
        }else{
            --size_t;
            int error = pthread_create(&thread[size_t],/*thread atributes*/ NULL, greet, /*function arguments*/ size_t);
            if (error == EXIT_SUCCESS){
                printf("Saludo desde: %i \n", size_t);
                //pthread_join(thread[size_t], NULL);
            }else {
                fprintf(stderr, "Error: could not create secondary thread");
            }
        }
        
    }
    pthread_join(thread[size_t], NULL);
    free(thread);
    return NULL;
} //end procedure
    