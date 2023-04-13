#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <assert.h>

#include "input_reader.h"

int returnsCorrectReadings_Test() {
    char inputStr[50] = "123456\n5\n\ntests/file1.zip\ntests/file2.zip";
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

int failWithNotASCIICharacters_Test() {
    return 0;
}

int failIfFilesDontExist_Test() {
    return 0;
}