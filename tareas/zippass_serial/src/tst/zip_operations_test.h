// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef ZIP_OPERATIONS_TEST_H
#define ZIP_OPERATIONS_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <assert.h>

#include "zip_operations.h"

int openWithNoPassword_Test();

int openWithPassword_Test();

#endif
