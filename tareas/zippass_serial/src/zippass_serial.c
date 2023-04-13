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
#include <assert.h>

#include "input_reader.h"
#include "password_logic.h"
#include "zip_operations_test.h"


/**********************************************************************************************************************
*    @brief Reads a string and determines if it's only composed of ASCII characters
*    @param input String containing the information related to the zip files to be decrypted
*    @return An error code:
*        0 for success
*        2 for input has at least one character not ASCII
***********************************************************************************************************************/
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

/**********************************************************************************************************************
*    @brief Asks the user for the input and validates it. After this it calls the subroutine to decipher the password. 
*    @param void Receives no parameters
*    @return An error code:
*        0 for success
*        int for any error
***********************************************************************************************************************/
int main(void) {
    int64_t maxCharacters = 999;
    char input[maxCharacters];
    printf("Please enter required information (use ; when finished): \n");
    int8_t error = scanf("%[^;]s",&input);
    if (error > 0) {
    } else {
        fprintf(stderr, "ERROR: Could not read input\n");
        return error;
    }
    error = validateASCIIInput(input);
    uint64_t lineCount = countChar(input, '\n');
    uint64_t fileCount = lineCount - 3;
    char** lines = lineator(input);
    char* alphabet = calloc(128, sizeof(char));
    alphabet = lines[0];
    alphabet = strcat(alphabet,"\0");
    uint64_t maxLength = strtoull(lines[1], NULL, 10);
    char** files = (char**) calloc(lineCount-3,sizeof(char*));
    char* file = (char*) calloc(128, sizeof(char));
    
    for (uint64_t line = 3; line < lineCount; line++) {
        file = lines[line];
        files[line - 3] = file;
    }
    
    // Decipher passwords
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    char* password = calloc(maxLength, sizeof(char));

    // Print whitespace for better visualization
    for (int8_t spaces = 0; spaces < 5; spaces++) {
        printf("\n");
    }
    
    
    for (uint64_t file = 0; file < fileCount; file++) {
        password = descipherPassword_Serial(files[file], alphabet, maxLength, password);
        printf("%s %s\n", files[file], password);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("Time: %.9lfs\n", elapsed_time);
    free(alphabet);
    free(lines);
    free(files);
    free(file);
    free(password);
    return error;
}