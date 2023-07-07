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


int8_t** generateNextPassword(int8_t* testCounters,
                               uint8_t sizeOfAlphabet,
                               uint8_t maxPwdLength,
                               uint8_t pwdLength,
                               int8_t** testCounterFlags);

char* translateCounterToPassword(int8_t* testCounters,
                                 char* alphabet,
                                 uint8_t pwdLength,
                                 char* password);

char* descipherPassword_Serial(char* file_path,
                               char* alphabet,
                               uint8_t maxPwdLength,
                               char* password);

#endif
