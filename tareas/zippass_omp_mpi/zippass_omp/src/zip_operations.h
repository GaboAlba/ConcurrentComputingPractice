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

/**********************************************************************************************************************
*    @brief Tries the password provided on the file provided 
*    
*    @param file_path       String containing the path to the file that must be unlocked
*    @param password        String that will be tested to unlock the encrypted file               
*
*    @return An error code
*       1 if password worked and unlocked file
*       0 if an error happened
*    
**********************************************************************************************************************/
int decrypt_zip(char* file_path, char* password);

#endif
