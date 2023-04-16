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

/**********************************************************************************************************************
*    @brief Tries the password provided on the file provided 
*    
*    @param file_path       String containing the path to the file that must be unlocked
*    @param password        String that will be tested to unlock the encrypted file               
*
*    @return An error code
*       1 if password worked and unlocked file
*       0 if an error happened
**********************************************************************************************************************/
int decrypt_zip(char* file_path, char* password) {
    int error;
    char* testString = "CI0117-23a";
    // Open ZIP file
    zip_t* file = zip_open(file_path, ZIP_RDONLY, &error);
    if (file == NULL) {
        // ERROR: Failed to open file
        return 0;
    }

    // Initialize metadata
    zip_stat_t zip_stats;
    zip_stat_init(&zip_stats);
    error = zip_stat_index(file, 0, ZIP_STAT_INDEX, &zip_stats);
    if (error != 0) {
        zip_discard(file);
        // "ERROR: Could not read file metadata"
        return 0;
    }

    // Read data from the compressed file
    zip_file_t* readFile = zip_fopen_index_encrypted(/*zip_t*/ file,
                                                    /*index*/ 0,
                                                    /*flag*/ ZIP_FL_NOCASE,
                                                    /*pwd*/ password);
    char* contents = calloc(zip_stats.size, sizeof(char));
    error = zip_fread(readFile, contents, zip_stats.size);
    if (error == -1) {
        // "ERROR: Could not read from compressed file\n"
        free(contents);
        zip_discard(file);
        return 0;
    }
    error = strcmp(testString, contents);
    if (error == EXIT_SUCCESS) {
    } else {
        free(contents);
        zip_fclose(readFile);
        zip_discard(file);
        // "ERROR: The value read from the file was not expected
        return 0;
    }

    free(contents);
    zip_fclose(readFile);
    zip_discard(file);
    return 1;
}

