#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <assert.h>
#include <string.h>

#include "zip_operations.h"

void openWithNoPassword_Test() {
    char* testString = "This test has no password";
    char* file_path = "tests/testfile0.zip";
    int64_t error = 0;
    zip_t* file = zip_open(file_path, ZIP_RDONLY, &error);
    assert(file != NULL);

    // Initialize metadata
    zip_stat_t zip_stats;
    zip_stat_init(&zip_stats);
    error = zip_stat_index(file, 0, ZIP_STAT_INDEX, &zip_stats);
    assert(error == 0);

    // Read the data inside
    zip_file_t* readFile = zip_fopen_index(file, 0, ZIP_FL_NOCASE);
    char* contents = calloc(zip_stats.size, sizeof(char));
    error = zip_fread(readFile, contents, zip_stats.size);
    assert(error != -1);
    error = strcmp(testString, contents);
    assert(error == 0);

    free(contents);
}

void openWithPassword_Test() {
    char* testString = "This is the first test";
    char* password = "@";
    char* file_path = "tests/testfile1.zip";
    int64_t error = 0;
    zip_t* file = zip_open(file_path, ZIP_RDONLY, &error);
    assert(file != NULL);

    //Initialize metadata
    zip_stat_t zip_stats;
    zip_stat_init(&zip_stats);
    error = zip_stat_index(file, 0, ZIP_STAT_INDEX, &zip_stats);
    assert(error == 0);

    // Read the data inside
    zip_file_t* readFile = zip_fopen_index_encrypted(file, 0, ZIP_FL_NOCASE, "@");
    char* contents = calloc(zip_stats.size, sizeof(char));
    error = zip_fread(readFile, contents, zip_stats.size);
    assert(error != -1);
    error = strcmp(testString, contents);
    assert(error == 0);

    free(contents);
}

void openTestCase() {
    char* testString = "CI0117-23a";
    char* password = "00112";
    char* file_path = "tests/zip_05/f01.zip";
    int64_t error = 0;
    zip_t* file = zip_open(file_path, ZIP_RDONLY, &error);
    assert(file != NULL);

    //Initialize metadata
    zip_stat_t zip_stats;
    zip_stat_init(&zip_stats);
    error = zip_stat_index(file, 0, ZIP_STAT_INDEX, &zip_stats);
    assert(error == 0);
    printf("Name: %s\n", zip_stats.name);
    printf("Index: %i\n", zip_stats.index);

    // Read the data inside
    zip_file_t* readFile = zip_fopen_index_encrypted(file, 0, ZIP_FL_NOCASE, password);
    char* contents = calloc(zip_stats.size, sizeof(char));
    error = zip_fread(readFile, contents, zip_stats.size);
    assert(error != -1);
    error = strcmp(testString, contents);
    assert(error == 0);

    free(contents);
}