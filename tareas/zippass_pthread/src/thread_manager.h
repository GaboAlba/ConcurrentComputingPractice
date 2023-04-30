// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>

#include "data_structures.h"

int createcreatePwdGeneratorThread(pwdGenData_t* pwdGenData);

int createFileTesterThread(testerThreadData_t* testerThreadData);

void createThreads(uint8_t numOfThreads, pwdGenData_t* pwdGenData, testerThreadData_t** testerThreadData);



#endif