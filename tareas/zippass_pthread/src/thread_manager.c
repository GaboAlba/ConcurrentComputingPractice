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


int createPwdGeneratorThread(pwdGenData_t* pwdGenData) {
    uint8_t QueueId = 0;
    uint8_t filesUnlocked = 0;
    // Define dynamic memory usage
    int8_t* testCounters = calloc(pwdGenData->publicData->maxPwdLength, sizeof(int8_t));
    char* lastPassword = calloc(pwdGenData->publicData->maxPwdLength, sizeof(char));
    char* password = calloc(pwdGenData->publicData->maxPwdLength, sizeof(char));
    for (uint8_t character = 0; character < pwdGenData->publicData->maxPwdLength; character++) {
        lastPassword[character] = pwdGenData->publicData->alphabet[pwdGenData->publicData->sizeOfAlphabet - 1];
    }
    bool exitCondition = false;

    // Init password array
    for (uint8_t character = 0; character < pwdGenData->publicData->maxPwdLength; character++) {
        if (character == 0) {
            password[character] = pwdGenData->publicData->alphabet[character];
            testCounters[character] = 0;
        } else {
            password[character] = '\0';
            testCounters[character] = -1;
        }
    }

    //Generate all passwords and enqueue them into the respective thread
    uint8_t pwdLength = 1;
    int8_t** testCounterFlags = calloc(2, sizeof(int8_t*));
    while (filesUnlocked < pwdGenData->publicData->fileCount) {
        // Introduce the password into each thread's queue
        enqueue(pwdGenData->QueueData[QueueId], password);

        // Generate a new password combination
        testCounterFlags = generateNextPassword(testCounters,
                                                pwdGenData->publicData->sizeOfAlphabet,
                                                pwdGenData->publicData->maxPwdLength,
                                                pwdLength,
                                                testCounterFlags);
        pwdLength = testCounterFlags[0];

        // Set password to the current combination
        password = translateCounterToPassword(testCounters,
                                              pwdGenData->publicData->alphabet,
                                              pwdLength,
                                              password);
        if (QueueId == pwdGenData->publicData->threadCount - 2) {
            QueueId = 0;
        } else {
            QueueId++;
        }

        if (!strcmp(password, lastPassword)) {
            //printf("->Enqueueing %s in queue %" PRIu64 "\n", password, QueueId);
            enqueue(pwdGenData->QueueData[QueueId], password);
            for (int queue = 0; queue < pwdGenData->publicData->threadCount - 1; queue++) {
                // If this is the last possible password, introduce an empty string as
                // the stop character
                enqueue(pwdGenData->QueueData[queue], "");
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

int createFileTesterThread(testerThreadData_t* testerThreadData) {
    char* password = calloc(testerThreadData->publicData->maxPwdLength, sizeof(char));
    bool exitCondition = false;
    while (testerThreadData->publicData->filesUnlocked < testerThreadData->publicData->fileCount && testerThreadData->threadDone == false) {
        if (!isQueueEmpty(testerThreadData->QueueData)) {
            // Grab value from that thread's queue
            password = dequeue(testerThreadData->QueueData);
            if (!strcmp(password,"")) {
                for (int file = 0; file < testerThreadData->publicData->fileCount; file++) {
                    if (testerThreadData->FilesData[file]->passwordFound == false) {
                        // Semaphore to make sure the files unlocked is not written at the same time
                        sem_wait(&testerThreadData->publicData->semaphore);
                        testerThreadData->publicData->filesUnlocked++;
                        testerThreadData->FilesData[file]->password = password;
                        testerThreadData->FilesData[file]->passwordFound = true;
                        testerThreadData->threadDone = true;
                        sem_post(&testerThreadData->publicData->semaphore);
                    }
                }
                return 1;
            }
            for (int file = 0; file < testerThreadData->publicData->fileCount; file++) {
                if (!testerThreadData->FilesData[file]->passwordFound) {
                    exitCondition = decrypt_zip(testerThreadData->publicData->fileList[file], password);
                    if (exitCondition) {
                        // Semaphore to make sure the files unlocked is not written at the same time
                        sem_wait(&testerThreadData->publicData->semaphore);
                        testerThreadData->publicData->filesUnlocked++;
                        sem_post(&testerThreadData->publicData->semaphore);
                        testerThreadData->FilesData[file]->password = password;
                        testerThreadData->FilesData[file]->passwordFound = true;
                    }
                }
            }
        }
    }
    return 0;
}

void createThreads(uint8_t numOfThreads, pwdGenData_t* pwdGenData, testerThreadData_t** testerThreadData) {
    pthread_t* threads = calloc(numOfThreads, sizeof(pthread_t));
    // Create password generator thread (PROVEN)
    int8_t error = pthread_create(&threads[0],
                                    NULL,
                                    createPwdGeneratorThread,
                                    pwdGenData);
    pthread_join(threads[0], NULL);
    // Create password tester threads
    for (int counter = 0; counter < numOfThreads - 1; counter++) {
        error = pthread_create(&threads[counter],
                                NULL,
                                createFileTesterThread,
                                testerThreadData[counter]);
    }
    // Wait for all threads to finish
    for (int counter = 0; counter < numOfThreads; counter++) {
        pthread_join(threads[counter], NULL);
    }
    free(threads);
}
