// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>
///  @file
///  @brief Main source program


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

/// @brief Struct that defines the data that can be accessed by any file and/or
///        thread
typedef struct publicData {
    char* alphabet;
    uint8_t maxLength;
} publicData_t;

/// @brief Struct that defines the data that is property of the file and/or
///        thread and may not be shared among them
typedef struct privateData {
    char* filePath;
    char* password;
    publicData_t* publicData;
} privateData_t;

//**********************************************************************************************************************
///    @brief Asks the user for the input and validates it. After this it calls
///           the subroutine to decipher the password.
///    @param void Receives no parameters
///    @return An error code:
///        @retval 0 for success
///        @retval int for any error
//**********************************************************************************************************************
int main(void) {
  const uint16_t kMaxCharacters = 999;
  char input[kMaxCharacters];
  printf("Please enter required information (use ; when finished): \n");
  int8_t error = scanf("%[^;]s", &input);
  if (error > 0) {
  } else {
    fprintf(stderr, "ERROR: Could not read input\n");
    return error;
  }
  error = validateASCIIInput(input);
  if (error) {
    return error;
  }
  uint8_t lineCount = countChar(input, '\n');
  uint8_t fileCount = lineCount - 3;
  char** lines = lineator(input);
  error = validateInputFormat(lines, lineCount);
  if (error) {
    return error;
  }
  publicData_t* publicData = calloc(1, sizeof(publicData_t));
  publicData->alphabet = calloc(128, sizeof(char));
  publicData->alphabet = lines[0];
  publicData->alphabet = strcat(publicData->alphabet, "\0");
  publicData->maxLength = strtoull(lines[1], NULL, 10);
  privateData_t* files = (privateData_t*) calloc(lineCount-3,
                                                 sizeof(privateData_t));

  // Assign files to a single data structure
  for (uint8_t line = 3; line < lineCount; line++) {
    files[line - 3].filePath = lines[line];
  }

  // Print whitespace for better visualization
  for (int8_t spaces = 0; spaces < 5; spaces++) {
    printf("\n");
  }
  printf("---Decrypting your password(s)---\n");

  // Start Timer
  struct timespec start_time, finish_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  // Decipher passwords
  for (uint8_t file = 0; file < fileCount; file++) {
    files[file].password = calloc(publicData->maxLength, sizeof(char));
    files[file].publicData = publicData;
    files[file].password = descipherPassword_Serial(files[file].filePath,
                                                    publicData->alphabet,
                                                    publicData->maxLength,
                                                    files[file].password);
    printf("%s %s\n", files[file].filePath, files[file].password);
  }

  // Stop taking time
  clock_gettime(CLOCK_MONOTONIC, &finish_time);
  double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
    (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

  printf("---Password(s) decrypted---\n");
  printf("Time: %.9lfs\n", elapsed_time);

  // Free up memory
  free(publicData);
  free(lines);
  free(files);
  return error;
}
