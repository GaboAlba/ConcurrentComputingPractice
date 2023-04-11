// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef ZIP_OPERATIONS_H
#define ZIP_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int decrypt_zip(char* file_path, char* password);

#endif