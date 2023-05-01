// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>
///  @file
///  @brief Main source program

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <semaphore.h>
#include <math.h>

#include "input_reader.h"
#include "password_logic.h"
#include "zip_operations_test.h"
#include "exec_time_test.h"
#include "queue_manager_test.h"
#include "data_structures.h"
#include "thread_manager.h"


void zippass_pthread(char** lines, uint8_t noOfThreads, uint8_t fileCount, uint8_t lineCount) {
    // Defining and initializing PublicData
    publicData_t* publicData = calloc(1, sizeof(publicData_t));
    publicData->alphabet = calloc(128, sizeof(char));
    publicData->alphabet = lines[0];
    publicData->alphabet = strcat(publicData->alphabet, "\0");
    publicData->maxPwdLength = strtoull(lines[1], NULL, 10);
    publicData->fileList = calloc(fileCount, sizeof(char*));
    publicData->fileCount = fileCount;
    publicData->threadCount = noOfThreads;
    publicData->sizeOfAlphabet = strlen(publicData->alphabet);
    publicData->filesUnlocked = 0;
    
    // Initializing semaphore
    sem_t threadsSemaphore;
    publicData->semaphore = threadsSemaphore;
    sem_init(&publicData->semaphore, 0, 1);

    // Initializing filesData
    FilesData_t** filesData = calloc(publicData->fileCount, sizeof(FilesData_t*));
    for (uint8_t line = 3; line < lineCount; line++) {
      publicData->fileList[line - 3] = lines[line];
      filesData[line - 3] = calloc(1, sizeof(FilesData_t));
      filesData[line - 3]->filePath = lines[line];
      filesData[line - 3]->password = calloc(publicData->maxPwdLength, sizeof(char));
      filesData[line - 3]->passwordFound = false;
    }

    // Initializing QueueData
    QueueData_t** queueData = calloc(publicData->threadCount - 1, sizeof(QueueData_t*));
    uint64_t maxNumOfPasswords = pow(publicData->sizeOfAlphabet, publicData->maxPwdLength);
    uint64_t maxQueueSize = maxNumOfPasswords * 2;
    for (uint8_t thread = 0; thread < noOfThreads - 1; thread++) {
      queueData[thread] = calloc(1, sizeof(QueueData_t));
      queueData[thread]->QueueMaxSize = maxQueueSize;
      queueData[thread]->QueueId = thread;
      queueData[thread] = createQueue(queueData[thread], thread);
    }

    // Initializing pwdGenData
    pwdGenData_t* pwdGenData = calloc(1, sizeof(pwdGenData_t));
    pwdGenData->publicData = publicData;
    pwdGenData->QueueData = queueData;
    pwdGenData->threadNumber = 0;

    // Initializing testerThreadData
    testerThreadData_t** testerThreadsData = calloc(noOfThreads - 1, sizeof(testerThreadData_t*));
    for (uint8_t thread = 0; thread < noOfThreads - 1; thread++) {
      testerThreadsData[thread] = calloc(1, sizeof(testerThreadData_t));
      testerThreadsData[thread]->FilesData = filesData;
      testerThreadsData[thread]->publicData = publicData;
      testerThreadsData[thread]->QueueData = queueData[thread];
      testerThreadsData[thread]->threadNumber = thread + 1;
    }


    // Print whitespace for better visualization
    for (int8_t spaces = 0; spaces < 5; spaces++) {
      printf("\n");
    }
    printf("---Decrypting your password(s) with %" PRIu8 " threads---\n", noOfThreads);

    // Start Timer
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Decipher passwords
    createThreads(noOfThreads, pwdGenData, testerThreadsData);

    // Stop taking time
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    // Print files and passwords
    for (int file = 0; file < fileCount; file++) {
      printf("%s %s\n", filesData[file]->filePath, filesData[file]->password);
    }

    printf("---Password(s) decrypted---\n");
    printf("Time: %.9lfs\n", elapsed_time);

    // Free up memory
    free(publicData);
    free(lines);
    free(pwdGenData);
    free(filesData);
    free(queueData);
    free(testerThreadsData);
}

void zippass_serial(char** lines, uint8_t fileCount, uint8_t lineCount) {

typedef struct privateData {
  char* filePath;
  char* password;
  publicData_t* publicData;
} privateData_t;

  publicData_t* publicData = calloc(1, sizeof(publicData_t));
  publicData->alphabet = calloc(128, sizeof(char));
  publicData->alphabet = lines[0];
  publicData->alphabet = strcat(publicData->alphabet, "\0");
  publicData->maxPwdLength = strtoull(lines[1], NULL, 10);
  privateData_t* files = (privateData_t*) calloc(lineCount-3,
                                                 sizeof(privateData_t));

  // Assign files to a single data structure
  for (uint8_t line = 3; line < lineCount; line++) {
    files[line - 3].filePath = lines[line];
  }

  // Print whitespace for better visualization
  for (int8_t spaces = 0; spaces < 5; spaces++) {
    printf("\n");
  }
  printf("---Decrypting your password(s)---\n");

  // Start Timer
  struct timespec start_time, finish_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  // Decipher passwords
  for (uint8_t file = 0; file < fileCount; file++) {
    files[file].password = calloc(publicData->maxPwdLength, sizeof(char));
    files[file].publicData = publicData;
    files[file].password = descipherPassword_Serial(files[file].filePath,
                                                    publicData->alphabet,
                                                    publicData->maxPwdLength,
                                                    files[file].password);
    printf("%s %s\n", files[file].filePath, files[file].password);
  }

  // Stop taking time
  clock_gettime(CLOCK_MONOTONIC, &finish_time);
  double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
    (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

  printf("---Password(s) decrypted---\n");
  printf("Time: %.9lfs\n", elapsed_time);

  // Free up memory
  free(publicData);
  free(lines);
  free(files);
}