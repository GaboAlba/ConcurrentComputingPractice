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


int64_t** generateNextPassword(int64_t* testCounters, uint64_t sizeOfAlphabet, uint64_t maxPwdLength, uint64_t pwdLength) {
    int64_t** testCountersFlags = calloc(2, sizeof(int64_t*));
    for (uint64_t cell = 0; cell < maxPwdLength; cell++) {
        if (testCounters[cell] >= sizeOfAlphabet - 1) {
            if(testCounters[cell + 1] == -1) {
                pwdLength++;
                testCounters[cell + 1] = 0;
                testCounters[cell] = 0;
                testCountersFlags[0] = (uint64_t*) pwdLength;
                testCountersFlags[1] = testCounters;
                return testCountersFlags;
            }
        } else {
            testCounters[cell]++;
            // Clean all previous cells to first character when changing that one
            for (int clears = cell - 1; clears >= 0; clears--) {
                testCounters[clears] = 0;
            }
            testCountersFlags[0] = (uint64_t*) pwdLength;
            testCountersFlags[1] = testCounters;
            return testCountersFlags;
        }
    }
    free(testCountersFlags);
}

char* translateCounterToPassword(uint64_t* testCounters, char* alphabet, uint64_t pwdLength) {
    char* password;
    for (uint64_t counter = 0; counter < pwdLength; counter++) {
            password[counter] = alphabet[testCounters[counter]];
        }
    return password;
}

char* descipherPassword_Serial(char* file_path, char* alphabet, uint64_t maxPwdLength) {
    char* password = calloc(maxPwdLength, sizeof(char));
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
        testCounterFlags = generateNextPassword(testCounters, sizeOfAlphabet, maxPwdLength, pwdLength);
        pwdLength = testCounterFlags[0];

        // Set password to the current combination
        password = translateCounterToPassword(testCounters, alphabet, pwdLength);
        printf("PWD: %s\n", password);

        // Throws error if password is longer the permitted
        if (pwdLength > maxPwdLength) {
            fprintf(stderr, "ERROR: The password generated is longer than permitted");
            return "FAILURE";
        }

        // Check if all passwords have been tried
        for (uint64_t counter = 0; counter < maxPwdLength; counter++) {
            if (testCounters[counter] == sizeOfAlphabet - 1) {
                exitCondition = true;
            } else {
                exitCondition = false;
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