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

/**********************************************************************************************************************
*    @brief Reads the current state of `testCounters` and determines the next password based on that information
*    
*    @param testCounters        Array of values representing the value of the alphabet in the current password characters
*    @param sizeOfAlphabet      Value that represents the size of the alphabet inputted in the standard input
*    @param maxPwdLength        Value that represents the maximum size of the password inputted in the standard input
*    @param pwdLength           Current password length
*    @param testCounterFlags    Array containing the current pwdLength and testCounters
**
*    @return Array containing next the new pwdLength and testCounters
*       testCounterFlags[2] 
**********************************************************************************************************************/
int64_t** generateNextPassword(int64_t* testCounters, uint64_t sizeOfAlphabet, uint64_t maxPwdLength, uint64_t pwdLength, int64_t** testCounterFlags);

/**********************************************************************************************************************
*    @brief Assigns the characters of the alphabet associated to the testCounter to the password for testing
*    
*    @param testCounters    @see generateNextPassword
*    @param alphabet        @see decipherPassword_Serial
*    @param pwdLength       @see generateNextPassword
*    @param password        Current password that has already been tested and will be modified 
*
*    @return Password to be tested in the zip file
*       password
**********************************************************************************************************************/
char* translateCounterToPassword(int64_t* testCounters, char* alphabet, uint64_t pwdLength, char* password);

/**********************************************************************************************************************
*    @brief Reads the encrypted file that has to be unlocked and searches for the password for the file
*    
*    @param file_path       String containing the path to the file that must be unlocked
*    @param alphabet        Array of all possible values that the password can have in it's characters.
*    @param maxPwdLength    Value that represents the maximum size of the password inputted in the standard input
*    @param password        Pointer to where the password will be saved during it's testing and return 
*
*    @return The string of the password that unlocks the file in the zip file
*       password if it was successfully determined
*       void if the password was not able to be detected    
**********************************************************************************************************************/
char* descipherPassword_Serial(char* file_path, char* alphabet, uint64_t maxPwdLength, char* password);

#endif