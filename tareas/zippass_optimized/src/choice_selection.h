// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef CHOICE_SELECTION_H
#define CHOICE_SELECTION_H

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

void zippass_pthread(char** lines, uint8_t noOfThreads,
                     uint8_t fileCount, uint8_t lineCount);

void zippass_serial(char** lines, uint8_t fileCount, uint8_t lineCount);

#endif
