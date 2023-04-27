
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

#include "password_logic.h"
#include "zip_operations.h"


/*
Program to test execution times of the different functions to find bottlenecks and possible improvements. 
Also, to decide best way to paralelize based on the ratio between passwordGenerators and passwordTesters
*/

// PasswordGenerator 5 digit test. 1000 iterations
void passwordGenTimer() {
    char* password = calloc(5, sizeof(char));
    char* alphabet = "0123456789";
    uint8_t sizeofAlphabet = 10;
    uint8_t maxPwdLength = 5;
    uint8_t pwdLength = 5;
    int8_t* testCounters = calloc(5, sizeof(int8_t));
    for (int counter = 0; counter < 5; counter++) {
        testCounters[counter] = 0;
    }
    int8_t** testCounterFlags = calloc(2, sizeof(int8_t*));
    testCounterFlags[0] = (int8_t*) pwdLength;
    testCounterFlags[1] = testCounters;

    // Init password array
    for (uint8_t character = 0; character < maxPwdLength; character++) {
        if (character == 0) {
            password[character] = alphabet[character];
            testCounters[character] = 0;
        }
        password[character] = '\0';
        testCounters[character] = -1;
    }

    // Start Timer
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for(uint64_t cycles = 0; cycles < 100000000; cycles++) {
        testCounterFlags = generateNextPassword(testCounters,
                                                sizeofAlphabet,
                                                maxPwdLength,
                                                pwdLength,
                                                testCounterFlags);
        pwdLength = testCounterFlags[0];
        password = translateCounterToPassword(testCounters,
                                              alphabet,
                                              pwdLength,
                                              password);
    }
    // Stop taking time
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("---PASSWORD GENERATOR---\n");
    printf("Time: %.9lfs\n", elapsed_time);
    free(password);
    free(testCounters);
    free(testCounterFlags);

}


// DecryptZip 5 digit WRONG password. 1000 iterations
void decryptTesterTimerWrong() {
    char* filePath = "tests/zip_05/f01.zip";
    char* password = "00113";
    // Start Timer
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for (uint16_t cycles = 0; cycles < 1000; cycles++) {
        decrypt_zip(filePath, password);
    }
    // Stop taking time
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("---WRONG PASSWORD DECRYPTION---\n");
    printf("Time: %.9lfs\n", elapsed_time);

}


// DecryptZip 5 digit RIGHT password. 1000 iterations
void decryptTesterTimerRight() {
    char* filePath = "tests/zip_05/f01.zip";
    char* password = "00112";
    // Start Timer
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for (uint16_t cycles = 0; cycles < 1000; cycles++) {
        decrypt_zip(filePath, password);
    }
    // Stop taking time
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("---RIGHT PASSWORD DECRYPTION---\n");
    printf("Time: %.9lfs\n", elapsed_time);
}


 // Test if a file can be open simultaneously
 void canFileBeOpenedMultipleTimes() {
    char* filePath = "tests/zip_05/f01.zip";
    char* password = "00113";
    int error;
    zip_t* file = zip_open(filePath, ZIP_RDONLY, &error);
    if (file == NULL) {
        printf("ERROR: Failed to open file");
        return 0;
    }
    zip_t* file1 = zip_open(filePath, ZIP_RDONLY, &error);
    if (file1 == NULL) {
        printf("ERROR: Failed to open file1");
        return 0;
    }
    zip_file_t* readFile = zip_fopen_index_encrypted(/*zip_t*/ file,
                                                    /*index*/ 0,
                                                    /*flag*/ ZIP_FL_NOCASE,
                                                    /*pwd*/ password);
    zip_file_t* readFile1 = zip_fopen_index_encrypted(/*zip_t*/ file1,
                                                    /*index*/ 0,
                                                    /*flag*/ ZIP_FL_NOCASE,
                                                    /*pwd*/ password);
    char* contents = calloc(20, sizeof(char));
    char* contents1 = calloc(20, sizeof(char));
    error = zip_fread(readFile, contents, 20);
    error = zip_fread(readFile1, contents1, 20);

    printf("It can!!\n");
 }