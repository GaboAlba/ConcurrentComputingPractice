// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "password_logic.h"
#include "zip_operations.h"

/**********************************************************************************************************************
*    @brief Reads the current state of `testCounters` and determines the next password based on that information
*    
*    @param testCounters        Array of values representing the value of the alphabet in the current password characters
*    @param sizeOfAlphabet      Value that represents the size of the alphabet inputted in the standard input
*    @param maxPwdLength        Value that represents the maximum size of the password inputted in the standard input
*    @param pwdLength           Current password length
*    @param testCounterFlags    Array containing the current pwdLength and testCounters
*
*    @details It takes a dynamic array of testCounters and determines the next value. If the position being searched for is the last value of the alphabet
*             it will initilize the next position of the array if the size permits it. In case the position being looked at is less than the last value of 
*             the alphabet then it will add one to that position. For example:
*              
*               With alphabet of size 10
*               testCounters = [9,9,9,NULL] -> [0,0,0,0]
*               testCounters = [0,4,7,8] -> [1,4,7,8]
*
*    @return Array containing next the new pwdLength and testCounters
*       testCounterFlags[2] 
*    
**********************************************************************************************************************/
int64_t** generateNextPassword(int64_t* testCounters, uint64_t sizeOfAlphabet, uint64_t maxPwdLength, uint64_t pwdLength, int64_t** testCountersFlags) {
    for (uint64_t cell = 0; cell < maxPwdLength; cell++) {
        if (testCounters[cell] >= sizeOfAlphabet - 1) {
            if(testCounters[cell + 1] == -1) {
                pwdLength++;
                testCounters[cell + 1] = 0;
                testCounters[cell] = 0;
                testCountersFlags[0] = (int64_t*) pwdLength;
                testCountersFlags[1] = testCounters;
                return testCountersFlags;
            }
        } else {
            testCounters[cell]++;
            // Clean all previous cells to first character when changing that one
            for (int clears = cell - 1; clears >= 0; clears--) {
                testCounters[clears] = 0;
            }
            testCountersFlags[0] = (int64_t*) pwdLength;
            testCountersFlags[1] = testCounters;
            return testCountersFlags;
        }
    }
    return testCountersFlags;
}


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
*    
**********************************************************************************************************************/
char* translateCounterToPassword(int64_t* testCounters, char* alphabet, uint64_t pwdLength, char* password) {
    for (uint64_t counter = 0; counter < pwdLength; counter++) {
            password[counter] = alphabet[testCounters[counter]];
        }
    return password;
}

/**********************************************************************************************************************
*    @brief Reads the current state of `testCounters` and determines the next password based on that information
*    
*    @param file_path       String containing the path to the file that must be unlocked
*    @param alphabet        Array of all possible values that the password can have in it's characters.
*    @param maxPwdLength    Value that represents the maximum size of the password inputted in the standard input
*    @param password        Pointer to where the password will be saved during it's testing and return 
*
*    @details It takes a dynamic array of testCounters and determines the next value. If the position being searched for is the last value of the alphabet
*             it will initilize the next position of the array if the size permits it. In case the position being looked at is less than the last value of 
*             the alphabet then it will add one to that position. For example:
*              
*               With alphabet of size 10
*               testCounters = [9,9,9,NULL] -> [0,0,0,0]
*               testCounters = [0,4,7,8] -> [1,4,7,8]
*
*    @return The string of the password that unlocks the file in the zip file
*       password if it was successfully determined
*       void if the password was not able to be detected  
*    
**********************************************************************************************************************/
char* descipherPassword_Serial(char* file_path, char* alphabet, uint64_t maxPwdLength, char* password) {
    //char* password = calloc(maxPwdLength, sizeof(char));
    int64_t* testCounters = calloc(maxPwdLength, sizeof(int64_t));
    uint64_t sizeOfAlphabet = strlen(alphabet);
    bool exitCondition = false;
    
    // init password array
    for (uint64_t character = 0; character < maxPwdLength; character++) {
        if (character == 0) {
            password[character] = alphabet[character];
            testCounters[character] = 0;
        }
        password[character] = NULL;
        testCounters[character] = -1;
    }

    // Brute force descipher password
    uint64_t pwdLength = 1;
    uint64_t** testCounterFlags = calloc(2, sizeof(uint64_t*));
    while (!exitCondition) {
            
        // Loop around the testCounters and look for the one that is not the maxCounter
        testCounterFlags = generateNextPassword(testCounters, sizeOfAlphabet, maxPwdLength, pwdLength, testCounterFlags);
        pwdLength = testCounterFlags[0];

        // Set password to the current combination
        password = translateCounterToPassword(testCounters, alphabet, pwdLength, password);
        //printf("PWD: %s\n", password);

        // Throws error if password is longer the permitted
        if (pwdLength > maxPwdLength) {
            fprintf(stderr, "ERROR: The password generated is longer than permitted");
            free(testCounters);
            free(testCounterFlags);
            return "FAILURE";
        }

        // Check if all passwords have been tried
        for (uint64_t counter = 0; counter < maxPwdLength; counter++) {
            if (testCounters[counter] == sizeOfAlphabet - 1) {
                exitCondition = true;
            } else {
                exitCondition = false;
                break;
            }
        }
        
        // Tries generated password if not all passwords have been tested
        if (!exitCondition) {
            exitCondition = decrypt_zip(file_path, password);
        } else {
            free(testCounters);
            free(testCounterFlags);
            return "";
        } 
        

    }
    free(testCounters);
    free(testCounterFlags);
    return password;
}