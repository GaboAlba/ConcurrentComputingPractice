// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>
// These tests are still under development, use them under your own risk

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <assert.h>

#include "password_logic.h"

void generatesNewPasswordCorrectlyChangingFirstNumber_Test() {
    // Current password is 197, next password should be 297
    char* alphabet = "0123456789";
    uint64_t maxPwdLength = 3;
    uint64_t PwdLength = 3;
    uint64_t nextPwdLength;
    uint64_t* currentCounters = calloc(3, sizeof(uint64_t));
    uint64_t* nextCounters = calloc(3, sizeof(uint64_t));
    currentCounters[0] = 1;
    currentCounters[1] = 9;
    currentCounters[2] = 7;
    uint64_t** nextCounterFlags = generateNextPassword(currentCounters,
                                                        alphabet,
                                                        maxPwdLength,
                                                        PwdLength,
                                                        nextCounterFlags);
    nextPwdLength = nextCounterFlags[0];
    nextCounters = nextCounterFlags[1];
    assert(nextPwdLength == PwdLength);
    assert(nextCounters[0] == 2);
    assert(nextCounters[1] == 9);
    assert(nextCounters[2] == 7);
    free(currentCounters);
    free(nextCounters);
    return 0;
}

void generatesNewPasswordCorrectlyAddingNumber_Test() {
    // Current password is 9, next password should be 00
    char* alphabet = "0123456789";
    uint64_t maxPwdLength = 3;
    uint64_t PwdLength = 3;
    uint64_t nextPwdLength;
    uint64_t* currentCounters = calloc(3, sizeof(uint64_t));
    uint64_t* nextCounters = calloc(3, sizeof(uint64_t));
    currentCounters[0] = 9;
    currentCounters[1] = NULL;
    currentCounters[2] = NULL;
    uint64_t** nextCounterFlags = generateNextPassword(currentCounters,
                                                        alphabet,
                                                        maxPwdLength,
                                                        PwdLength,
                                                        nextCounterFlags);
    nextPwdLength = nextCounterFlags[0];
    nextCounters = nextCounterFlags[1];
    assert(nextPwdLength == PwdLength);
    assert(nextCounters[0] == 0);
    assert(nextCounters[1] == 0);
    assert(nextCounters[2] == NULL);
    free(currentCounters);
    free(nextCounters);
}

void interpretsCountersCorrectly() {
    // Need to implement in future
}

void decryptsSimplePasswords_Test() {
    // Need to implement in future
}

