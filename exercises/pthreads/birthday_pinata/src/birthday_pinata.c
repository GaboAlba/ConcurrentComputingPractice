// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <semaphore.h>

typedef struct publicData {
    uint64_t hitsRemaining;
    uint64_t threadTotal;
    sem_t mutex;
} publicData_t;

typedef struct privateData {
    uint64_t thread_number;
    publicData_t* publicData;
} privateData_t;

void* hitPinata(void* data);

// procedure main:
int main(int argc, char* argv[]) {

    int error = EXIT_SUCCESS;
    char* ptr;
    if (argv[1] == NULL || argv[2] == NULL ) {
        fprintf(stderr, "ERROR -1: Input cannot be empty\n");
        error = -1;
        return error;
    }
    srand(time(NULL));
    uint64_t pinataResistance = strtol(argv[2], &ptr, 10);
    uint64_t threadsInvited = strtol(argv[1], &ptr, 10);
    pthread_t* threads = (pthread_t*) calloc(threadsInvited, sizeof(pthread_t));
    privateData_t* privateData = (privateData_t*) calloc(threadsInvited, sizeof(privateData_t)); 
    publicData_t* publicData = (publicData_t*) calloc(1, sizeof(publicData_t));
    publicData->hitsRemaining = pinataResistance;
    publicData->threadTotal = threadsInvited;
    sem_init(&publicData->mutex, 0, 1);
    for (uint64_t thread = 0; thread < threadsInvited; thread++) {
        privateData[thread].thread_number = thread;
        privateData[thread].publicData = publicData;
        error = pthread_create(&threads[thread], NULL, hitPinata, &privateData[thread]);
        if (error == EXIT_SUCCESS) {
        } else {
            fprintf(stderr, "ERROR: could not create secondary thread %" PRIu64 "\n", thread);
        }
    }

    for (uint64_t thread = 0; thread < threadsInvited; thread++) {
        pthread_join(threads[thread], NULL);
    }
    sem_destroy(&publicData->mutex);
    free(privateData);
    free(publicData);
    free(threads);
    return EXIT_SUCCESS;
}
// end procedure

void* hitPinata(void* data){
    privateData_t* privateData = (privateData_t*) data;
    publicData_t* publicData = (publicData_t*) privateData->publicData;
    sem_wait(&publicData->mutex);
    if (publicData->hitsRemaining <= 0) {
        printf("Thread %" PRIu64 "/%" PRIu64 ": The pinata was already broken\n", privateData->thread_number, publicData->threadTotal);
    } else {
        uint64_t hits = rand() % publicData->hitsRemaining + 1;
        publicData->hitsRemaining = publicData->hitsRemaining - hits;
        if (publicData->hitsRemaining > 0) {
            printf("Thread %" PRIu64 "/%" PRIu64 ":%" PRIu64 " hits\n", privateData->thread_number, publicData->threadTotal, hits);
        } else {
            printf("Thread %" PRIu64 "/%" PRIu64 ":%" PRIu64 " hits. I broke the pinata\n",privateData->thread_number, publicData->threadTotal, hits);
        }
    }
    
    sem_post(&publicData->mutex);
    return NULL;

}



