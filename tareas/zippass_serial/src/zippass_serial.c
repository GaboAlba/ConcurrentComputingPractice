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
    char** lines = lineator(input);
    char* alphabet = lines[0];
    alphabet = strcat(alphabet,"\0");
    uint64_t maxLength = strtoull(lines[1], NULL, 10);
    char** files = (char**) calloc(lineCount-3,sizeof(char*));
    for (uint64_t line = 3; line < lineCount; line++) {
        files[line - 3] = lines[line];
    }
    // Decipher passwords
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    char** passwords = calloc(fileCount, sizeof(char*));
    for (uint64_t file = 0; file < fileCount; file++) {
        passwords[file] = descipherPassword_Serial(files[file], alphabet, maxLength);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    for (uint64_t password = 0; password < fileCount; password++) {
        printf("%s %s\n", files[password], passwords[password]);
    }
    printf("Time: %.9lfs\n", elapsed_time);
    free(files);
    free(passwords);
    return error;
}