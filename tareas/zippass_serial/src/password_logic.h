// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef PASSWORD_LOGIC_H
#define PASSWORD_LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "zip_operations.h"


int64_t** generateNextPassword(int64_t* testCounters,
                               uint64_t sizeOfAlphabet,
                               uint64_t maxPwdLength,
                               uint64_t pwdLength,
                               int64_t** testCounterFlags);

char* translateCounterToPassword(int64_t* testCounters,
                                 char* alphabet,
                                 uint64_t pwdLength,
                                 char* password);

char* descipherPassword_Serial(char* file_path,
                               char* alphabet,
                               uint64_t maxPwdLength,
                               char* password);

#endif
