// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include "data_structures.h"
#include "queue_manager.h"
#include "password_logic.h"


int createPwdGeneratorThread(threadData_t* threadsData) {
    uint8_t QueueId = 0;
    uint8_t filesUnlocked = 0;
    // Define dynamic memory usage
    int8_t* testCounters = calloc(threadsData->publicData->maxPwdLength, sizeof(int8_t));
    char* lastPassword = calloc(threadsData->publicData->maxPwdLength, sizeof(char));
    char* password = calloc(threadsData->publicData->maxPwdLength, sizeof(char));
    for (uint8_t character = 0; character < threadsData->publicData->maxPwdLength; character++) {
        lastPassword[character] = threadsData->publicData->alphabet[threadsData->publicData->sizeOfAlphabet - 1];
    }
    bool exitCondition = false;

    // Init password array
    for (uint8_t character = 0; character < threadsData->publicData->maxPwdLength; character++) {
        if (character == 0) {
            password[character] = threadsData->publicData->alphabet[character];
            testCounters[character] = 0;
        }
        password[character] = '\0';
        testCounters[character] = -1;
    }

    //Generate all passwords and enqueue them into the respective thread
    uint8_t pwdLength = 1;
    int8_t** testCounterFlags = calloc(2, sizeof(int8_t*));
    while (filesUnlocked < threadsData->publicData->fileCount) {
        // Introduce the password into each thread's queue
        enqueue(threadsData[QueueId].QueueData, password);

        // Generate a new password combination
        testCounterFlags = generateNextPassword(testCounters,
                                                threadsData->publicData->sizeOfAlphabet,
                                                threadsData->publicData->maxPwdLength,
                                                pwdLength,
                                                testCounterFlags);
        pwdLength = testCounterFlags[0];

        // Set password to the current combination
        password = translateCounterToPassword(testCounters,
                                              threadsData->publicData->alphabet,
                                              pwdLength,
                                              password);
        if (QueueId == threadsData->publicData->threadCount - 2) {
            QueueId = 0;
        } else {
            QueueId++;
        }

        if (!strcmp(password, lastPassword)) {
            for (int queue = 0; queue < threadsData->publicData->threadCount - 2; queue++) {
                // If this is the last possible password, introduce an empty string as
                // the stop character
                enqueue(threadsData[queue].QueueData, "");
            }
            break;
        }

    }
    free(password);
    free(lastPassword);
    free(testCounters);
    free(testCounterFlags);
    return 0;
}

int createFileTesterThread(threadData_t threadData) {
    char* password = calloc(threadData.publicData->maxPwdLength, sizeof(char));
    bool exitCondition = false;
    while (threadData.publicData->filesUnlocked < threadData.publicData->fileCount) {
        // Grab value from that thread's queue
        password = dequeue(threadData.QueueData);
        if (!strcmp(password,"")) {
            for (int file = 0; file < threadData.publicData->fileCount; file++) {
                if (threadData.FilesData[file]->passwordFound == false) {
                // Semaphore to make sure the files unlocked is not written at the same time
                sem_wait(&threadData.publicData->semaphore);
                threadData.publicData->filesUnlocked++;
                threadData.FilesData[file]->password = password;
                threadData.FilesData[file]->passwordFound = true;
                sem_post(&threadData.publicData->semaphore);
                }
            }
            pthread_exit(NULL);
        }
        for (int file = 0; file < threadData.publicData->fileCount; file++) {
            exitCondition = decrypt_zip(threadData.publicData->fileList[file], password);
            if (exitCondition) {
                // Semaphore to make sure the files unlocked is not written at the same time
                sem_wait(&threadData.publicData->semaphore);
                threadData.publicData->filesUnlocked++;
                sem_post(&threadData.publicData->semaphore);
                threadData.FilesData[file]->password = password;
                threadData.FilesData[file]->passwordFound = true;
            }
        }
        
    }
    free(password);
    return 0;
}

void createThreads(uint8_t numOfThreads, threadData_t* threadsData) {
    pthread_t* threads = calloc(numOfThreads, sizeof(pthread_t));
    // Create password generator thread
    int8_t error = pthread_create(&threads[0],
                                    NULL,
                                    createPwdGeneratorThread,
                                    &threadsData);
    // Create password tester threads
    for (int counter = 1; counter < numOfThreads; counter++) {
        error = pthread_create(&threads[counter],
                                NULL,
                                createPwdGeneratorThread,
                                &threadsData[counter]);
    }
    // Wait for all threads to finish
    for (int counter = 0; counter < numOfThreads; counter++) {
        pthread_join(threads[counter], NULL);
    }

}
