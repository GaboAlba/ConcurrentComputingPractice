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
#include <omp.h>

#include "thread_manager.h"
#include "data_structures.h"
#include "queue_manager.h"
#include "password_logic.h"

//*****************************************************************************
/// @brief Manages the logic for the Password Generator thread. Generates new
///        passwords and enqueues them into the File Tester Queues
///
/// @param pwdGenData   pwdGenData type generated during choice selection that
///                     contains Queues array, public data and thread No.
///
/// @details It iterates creating a new password every cycle. The generated
///          password is then enqueued in a particular queue associated with
///          one of the File Testers. It cycles through the queues until either
///          the password is found for all files or the last possible password
///          is generated.
///
/// @return Error code with 0 being pass and any other number an error code.
///
//*****************************************************************************
int createPwdGeneratorThread(pwdGenData_t* pwdGenData) {
  uint8_t QueueId = 0;
  uint8_t filesUnlocked = 0;
  // Define dynamic memory usage
  int8_t* testCounters = calloc(pwdGenData->publicData->maxPwdLength,
                                  sizeof(int8_t));
  char* lastPassword = calloc(pwdGenData->publicData->maxPwdLength,
                              sizeof(char));
  char* password = calloc(pwdGenData->publicData->maxPwdLength,
                          sizeof(char));
  for (uint8_t character = 0; character < pwdGenData->publicData->maxPwdLength;
                                                                 character++) {
    lastPassword[character] = pwdGenData->publicData->alphabet[
                                                      pwdGenData->
                                                      publicData->
                                                      sizeOfAlphabet - 1];
  }

  // Init password array
  for (uint8_t character = 0; character < pwdGenData->publicData->maxPwdLength;
                                                                 character++) {
    if (character == 0) {
        password[character] = pwdGenData->publicData->alphabet[character];
        testCounters[character] = 0;
    } else {
        password[character] = '\0';
        testCounters[character] = -1;
    }
  }

  // Generate all passwords and enqueue them into the respective thread
  uint8_t pwdLength = 1;
  int8_t** testCounterFlags = calloc(2, sizeof(int8_t*));
  while (filesUnlocked < pwdGenData->publicData->fileCount) {
    // Introduce the password into each thread's queue
    enqueue(pwdGenData->QueueData[QueueId], password);

    // Generate a new password combination
    testCounterFlags = generateNextPassword(testCounters,
                                            pwdGenData->publicData->
                                                        sizeOfAlphabet,
                                            pwdGenData->publicData->
                                                        maxPwdLength,
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
      enqueue(pwdGenData->QueueData[QueueId], password);
      for (uint8_t queue = 0; queue < pwdGenData->publicData->threadCount - 1;
                                                                     queue++) {
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


//*****************************************************************************
/// @brief Manages the logic of a single File Tester thread. Different from the
///        non-OMP version, it also creates subthreads to decrypt all files at
///        the same time, bringing a huge increase in performance
///
/// @param testerThreadData  testerThreadData type that was generated during
///                          choice selection based on the number of threads
///
/// @details It dequeues a password from their own queue and tests it on all
///          files serially. If the password works it writes that password into
///          that file's data and changes it's passwordFound flag to true in
///          order for that file to not keep being tested even by other threads
///
/// @return Error code with 0 being pass and any other number an error code.
//*****************************************************************************
int createFileTesterThread_OMP(testerThreadData_t* testerThreadData) {
  char* password = calloc(testerThreadData->publicData->maxPwdLength,
                          sizeof(char));
  bool exitCondition = false;
  while (testerThreadData->publicData->filesUnlocked <
    testerThreadData->publicData->fileCount || testerThreadData->threadDone) {
    if (!isQueueEmpty(testerThreadData->QueueData)) {
      // Grab value from that thread's queue
      password = dequeue(testerThreadData->QueueData);
      if (!strcmp(password, "") || testerThreadData->QueueData->front ==
                                    testerThreadData->QueueData->rear) {
        // Semaphore to write to threadsDone safely
        #pragma omp critical
        {
          testerThreadData->publicData->testerThreadsDone++;
          testerThreadData->threadDone = true;
        }
        // Only if all passwords have been tested assign no password to the
        // remaining files
        if (testerThreadData->publicData->testerThreadsDone ==
            testerThreadData->publicData->threadCount) {
          for (int file = 0; file < testerThreadData->publicData->fileCount;
                                                                      file++) {
            if (testerThreadData->FilesData[file]->passwordFound == false) {
              // Semaphore to make sure the files unlocked is not written at
              // the same time
              #pragma omp critical
              {
                testerThreadData->publicData->filesUnlocked++;
                testerThreadData->FilesData[file]->password = password;
                testerThreadData->FilesData[file]->passwordFound = true;
              }
            }
          }
        }
        return 1;
      }
      for (int file = 0; file < testerThreadData->publicData->fileCount;
                                                                      file++) {
        if (!testerThreadData->FilesData[file]->passwordFound) {
          exitCondition = decrypt_zip(testerThreadData->publicData->
                                                        fileList[file],
                                                        password);
          if (exitCondition) {
            // Semaphore to make sure the files unlocked is not written at the
            // same time
            #pragma omp critical
            {
              testerThreadData->publicData->filesUnlocked++;
              testerThreadData->FilesData[file]->password = password;
              testerThreadData->FilesData[file]->passwordFound = true;
            }
          }
        }
      }
    }
  }
  return 0;
}


//*****************************************************************************
/// @brief Manages the logic of a single File Tester thread.
///
/// @param testerThreadData  testerThreadData type that was generated during
///                          choice selection based on the number of threads
///
/// @details It dequeues a password from their own queue and tests it on all
///          files serially. If the password works it writes that password into
///          that file's data and changes it's passwordFound flag to true in
///          order for that file to not keep being tested even by other threads
///
/// @return Error code with 0 being pass and any other number an error code.
//*****************************************************************************
int createFileTesterThread(testerThreadData_t* testerThreadData) {
  char* password = calloc(testerThreadData->publicData->maxPwdLength,
                          sizeof(char));
  bool exitCondition = false;
  while (testerThreadData->publicData->filesUnlocked <
    testerThreadData->publicData->fileCount || testerThreadData->threadDone) {
    if (!isQueueEmpty(testerThreadData->QueueData)) {
      // Grab value from that thread's queue
      password = dequeue(testerThreadData->QueueData);
      if (!strcmp(password, "") || testerThreadData->QueueData->front ==
                                    testerThreadData->QueueData->rear) {
        // Semaphore to write to threadsDone safely
        sem_wait(&testerThreadData->publicData->semaphore);
        testerThreadData->publicData->testerThreadsDone++;
        testerThreadData->threadDone = true;
        sem_post(&testerThreadData->publicData->semaphore);
        // Only if all passwords have been tested assign no password to the
        // remaining files
        if (testerThreadData->publicData->testerThreadsDone ==
            testerThreadData->publicData->threadCount) {
          for (int file = 0; file < testerThreadData->publicData->fileCount;
                                                                      file++) {
            if (testerThreadData->FilesData[file]->passwordFound == false) {
              // Semaphore to make sure the files unlocked is not written at
              // the same time
              sem_wait(&testerThreadData->publicData->semaphore);
              testerThreadData->publicData->filesUnlocked++;
              testerThreadData->FilesData[file]->password = password;
              testerThreadData->FilesData[file]->passwordFound = true;
              sem_post(&testerThreadData->publicData->semaphore);
            }
          }
        }
        return 1;
      }
      for (int file = 0; file < testerThreadData->publicData->fileCount;
                                                                      file++) {
        if (!testerThreadData->FilesData[file]->passwordFound) {
          exitCondition = decrypt_zip(testerThreadData->publicData->
                                                        fileList[file],
                                                        password);
          if (exitCondition) {
            // Semaphore to make sure the files unlocked is not written at the
            // same time
            sem_wait(&testerThreadData->publicData->semaphore);
            testerThreadData->publicData->filesUnlocked++;
            testerThreadData->FilesData[file]->password = password;
            testerThreadData->FilesData[file]->passwordFound = true;
            sem_post(&testerThreadData->publicData->semaphore);
          }
        }
      }
    }
  }
  return 0;
}

//*****************************************************************************
/// @brief Creates the Password Generator thread and the File Tester threads.
///
/// @param numOfThreads      The number of threads that must be generated
///
/// @param pwdGenData        pwdGenData type generated during choice selection
///                          that contains Queues array, public data and
///                          thread No.
///
/// @param testerThreadData  testerThreadData type generated during choice
///                          selection based on the number of threads that are
///                          going to be used.
///
//*****************************************************************************
void createThreads(uint8_t numOfThreads,
                   pwdGenData_t* pwdGenData,
                   testerThreadData_t** testerThreadData) {
    // Declar and init threads
    pthread_t* threads = calloc(numOfThreads, sizeof(pthread_t));
    // Create password generator thread
    int8_t error = pthread_create(&threads[0],
                                    NULL,
                                    createPwdGeneratorThread,
                                    pwdGenData);
    if (error == EXIT_SUCCESS) {
    } else {
      fprintf(stderr, "ERROR: The Password Generator thread was not created");
    }
    // Create password tester threads
    for (int counter = 1; counter < numOfThreads; counter++) {
      error = pthread_create(&threads[counter],
                              NULL,
                              createFileTesterThread,
                              testerThreadData[counter - 1]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "ERROR: A File Tester thread was not created");
      }
    }
    // Wait for all threads to finish
    for (int counter = 0; counter < numOfThreads; counter++) {
        pthread_join(threads[counter], NULL);
    }
    free(threads);
}

//*****************************************************************************
/// @brief Creates the Password Generator thread and the File Tester threads.
///
/// @param numOfThreads      The number of threads that must be generated
///
/// @param pwdGenData        pwdGenData type generated during choice selection
///                          that contains Queues array, public data and
///                          thread No.
///
/// @param testerThreadData  testerThreadData type generated during choice
///                          selection based on the number of threads that are
///                          going to be used.
///
//*****************************************************************************
void createThreads_OMP(uint8_t numOfThreads,
                   pwdGenData_t* pwdGenData,
                   testerThreadData_t** testerThreadData) {
    // Create password generator thread
    createPwdGeneratorThread(pwdGenData);
    // Create password tester threads
    #pragma omp parallel num_threads(numOfThreads) \
    default(none) shared(numOfThreads, testerThreadData)
    { // NOLINT
      #pragma omp for
      for (int counter = 1; counter < numOfThreads; counter++) {
        createFileTesterThread_OMP(testerThreadData[counter - 1]);
      }
    }
}
