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

#include "zip_operations.h"

int read_zip_contents() {
    return 0;
}

int decrypt_zip(char* file_path, char* password) {
    int error;
    char* testString = "CI0117-23a";
    // Open ZIP file
    zip_t* file = zip_open(file_path, ZIP_RDONLY, &error);
    if (file == NULL) {
        fprintf(stderr, "ERROR: Failed to open file %s\n", zip_strerror(file));
        return 0;
    }

    // Initialize metadata
    zip_stat_t zip_stats;
    zip_stat_init(&zip_stats);
    error = zip_stat_index(file, 0, ZIP_STAT_INDEX, &zip_stats);
    if (error != 0) {
        fprintf(stderr, "ERROR: Could not read file metadata\n");
        return 0;
    }

    //Read data from the compressed file
    zip_file_t* readFile = zip_fopen_index_encrypted(file, 0, ZIP_FL_NOCASE, password);
    char* contents = calloc(zip_stats.size, sizeof(char));
    error = zip_fread(readFile, contents, zip_stats.size);
    if (error == -1) {
        // Uncomment in case of troubleshooting
        //fprintf(stderr, "ERROR: Could not read from compressed file\n");
        free(contents);
        return 0;
    }
    error = strcmp(testString, contents);
    if (error == EXIT_SUCCESS) {
    } else {
        free(contents);
        fprintf(stderr, "ERROR: The value read from the file was not expected\n");
        return 0;
    }

    free(contents);
    zip_close(file);
    return 1;
}

