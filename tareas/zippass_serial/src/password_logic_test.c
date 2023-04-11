#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <assert.h>

#include "password_logic.h"

int generatesNewPasswordCorrectlyChangingFirstNumber_Test() {
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
    uint64_t** nextCounterFlags = generateNextPassword(currentCounters, alphabet, maxPwdLength,  PwdLength);
    nextPwdLength = nextCounterFlags[0];
    nextCounters = nextCounterFlags[1];
    assert(nextPwdLength == PwdLength);
    assert(nextCounters[0] == 2);
    assert(nextCounters[1] == 9);
    assert(nextCounters[2] == 7);
    return 0;
}

int generatesNewPasswordCorrectlyAddingNumber_Test() {
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
    uint64_t** nextCounterFlags = generateNextPassword(currentCounters, alphabet, maxPwdLength,  PwdLength);
    nextPwdLength = nextCounterFlags[0];
    nextCounters = nextCounterFlags[1];
    assert(nextPwdLength == PwdLength);
    assert(nextCounters[0] == 0);
    assert(nextCounters[1] == 0);
    assert(nextCounters[2] == NULL);
    return 0;
}

void interpretsCountersCorrectly() {
    
}

int decryptsSimplePasswords_Test() {
    return 0;
}

int generateAllPossiblePwds_tests() {
    char* alphabet = "abc";
    uint64_t maxLength = 3;
    char** combinations;
    combinations[0] = "a";
    combinations[1] = "b";
    combinations[2] = "c";
    combinations[3] = "aa";
    combinations[4] = "ba";
    combinations[5] = "ca";
    combinations[6] = "ab";
    combinations[7] = "bb";
    combinations[8] = "cb";
    combinations[9] = "cc";
    combinations[10] = "aaa";
    combinations[11] = "baa";
    combinations[12] = "caa";
    combinations[13] = "aba";
    combinations[14] = "bba";
    combinations[15] = "cba";
    combinations[16] = "aca";
    combinations[17] = "bca";
    combinations[18] = "cca";
    combinations[19] = "aab";
    combinations[20] = "bab";
    combinations[21] = "cab";
    combinations[22] = "abc";
    combinations[23] = "bbc";
    combinations[24] = "cbc";
    combinations[25] = "ccc";
}
