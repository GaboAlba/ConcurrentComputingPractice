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

#include "input_reader.h"


int validateASCIIInput(char input[]) {
    uint64_t inputSize = strlen(input);
    for (uint32_t character = 0; character < inputSize; character++) {
        int error = __isascii(input[character]);
        if (error) {
        } else {
            fprintf(stderr,"ERROR: Character %c is not ASCII\n",input[character]);
            error = 2;
            break;
        }
    }
    return 0;
}

int validatePrerequisites() {
    int error = system("zip -h >/dev/null 2>&1");
    return error;
}

bool decrypt_zip(char* file_path, char* password) {
    int error;
    zip_t* file = zip_open(file_path, ZIP_RDONLY, &error);
    if (file == NULL) {
        fprintf(stderr, "ERROR: Failed to open file %s\n", zip_strerror(file));
        return false;
    }
}

char* generateNextPassword(uint64_t* testCounters, char* alphabet) {

}

char* descipherPassword_Serial(char* file_path, char* alphabet, uint64_t maxPwdLength) {
    char* password = calloc(maxPwdLength, sizeof(char));
    uint64_t* testCounters = calloc(maxPwdLength, sizeof(uint64_t));
    uint64_t sizeOfAlphabet = strlen(alphabet);
    bool exitCondition = false;
    bool success = false;
    
    // init password array
    for (uint64_t character = 0; character < maxPwdLength; character++) {
        if (character == 0) {
            password[character] = alphabet[character];
            testCounters[character] = 0;
        }
        password[character] = NULL;
        testCounters[character] = NULL;
    }

    // Brute force descipher password
    uint64_t pwdLength = 1;
    while (!exitCondition) {
        // Loop around the testCounters and look for the one that is not the maxCounter
        for (uint64_t cell = 0; cell < maxPwdLength; cell++) {
            if (testCounters[cell] == sizeOfAlphabet - 1) {
                if(testCounters[cell + 1] == NULL) {
                    pwdLength++;
                    testCounters[cell + 1] = 0;
                    break;
                }
            } else {
                testCounters[cell]++;
                // Clean all previous cells to first character when changing that one
                for (uint64_t clears = cell - 1; clears >= 0; clears--) {
                    testCounters[clears] = 0;
                }
                break;
            }
        }

        // Set password to the current combination
        for (uint64_t counter = 0; counter < pwdLength; counter++) {
            password[counter] = alphabet[testCounters[counter]];
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
            return "";
        }
        

        
        
    }
    
    return password;
}

/*@brief To be implemented */
char* descipherPassword_Parallel(char* file_path) {
    return "PASS";
}

int main(void) {
    int64_t maxCharacters = 999;
    int8_t error = validatePrerequisites();
    if (error == EXIT_SUCCESS) {
    } else {
        fprintf(stderr, "ERROR: Please install required prerequistes");
    }
    char input[maxCharacters];
    printf("Please enter required information (use ; when finished): \n");
    error = scanf("%[^;]s",&input);
    if (error > 0) {
    } else {
        fprintf(stderr, "ERROR: Could not read input\n");
        return error;
    }
    error = validateASCIIInput(input);
    uint64_t lineCount = countChar(input, '\n');
    uint64_t fileCount = lineCount - 3;
    printf("LC: %" PRIu64 "\n", lineCount);
    char** lines = lineator(input);
    char* alphabet = lines[0];
    uint64_t maxLength = strtoull(lines[1], NULL, 10);
    char** files = (char**) calloc(lineCount-3,sizeof(char*));
    for (uint64_t line = 3; line < lineCount; line++) {
        files[line - 3] = lines[line];
        printf("Registered: %s\n", files[line-3]);
    }
    printf("ABC: %s\n",alphabet);
    printf("Length: %" PRIu64 "\n" , maxLength);
    printf("FirstFile: %s\n", files[0]);
    printf("SecondFile: %s\n", files[1]);

    for (uint64_t file = 0; file < fileCount; file++) {
        descipherPassword_Serial(files[file], alphabet, maxLength);
    }

    free(files);
    return error;
}