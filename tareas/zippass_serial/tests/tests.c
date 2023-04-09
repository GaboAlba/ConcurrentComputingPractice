#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <assert.h>

#include "../src/input_reader.h"

int returnsCorrectReadings_Test() {
    char inputStr[40] = "123456\n5\n\ntests/file1.zip\ntests/file2.zip";
    char line1[6] = "123456";
    char line2[1] = "5";
    char line3[1] = "";
    char line4[15] = "tests/file1.zip";
    char line5[15] = "tests/file2.zip";
    char** lines = lineator(inputStr);
    assert(lines[0] == line1);
    assert(lines[1] == line2);
    assert(lines[2] == line3);
    assert(lines[3] == line4);
    assert(lines[4] == line5);
    return 0;
}

int openWithNoPassword_Test() {
    char* zipfile = "testfile0.zip";
    int error;
    struct zip* file = zip_open(zipfile, ZIP_CHECKCONS, &error);
    assert(file != NULL);
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

int openWithPassword_Test() {
    return 0;
}

int failWithNotASCIICharacters_Test() {
    return 0;
}

int failIfFilesDontExist_Test() {
    return 0;
}

int main() {
    //returnsCorrectReadings_Test();
    openWithNoPassword_Test();
    return 0;
}