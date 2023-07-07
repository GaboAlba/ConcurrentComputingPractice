// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>
///  @file
///  @brief Main source program

/// @brief Constant that can be changed for debugging purposes. Set to 1 for
///        running tests, set to 0 for running the decrypter.
#define DEBUGCONST 0

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#include "choice_selection.h"
#include "input_reader.h"
#include "zip_operations_test.h"
#include "exec_time_test.h"
#include "queue_manager_test.h"

//*****************************************************************************
///    @brief Asks the user for the input and validates it. After this it calls
///           the subroutine to decipher the password. Also can be used for
///           testing purposes with the unit tests provided.
///    @param argc A counter of the amount of inputs provided from the stdin
///    @param argv An array of the inputs provided, in this case it corresponds
///                to the amount of threads that are going to be used
//*****************************************************************************
int main(int argc, char* argv[]) {
  if (!DEBUGCONST) {
    char* parallelForm = calloc(8, sizeof(char));
    uint8_t noOfThreads;
    if (argv[1] != NULL && argv[2] != NULL) {
      parallelForm = argv[1];
      noOfThreads = strtoull(argv[2], NULL, 10);
    } else if (validateLength(argv[1]) && argv[2] == NULL) {
      parallelForm = argv[1];
      noOfThreads = sysconf(_SC_NPROCESSORS_ONLN);
    } else if (!validateLength(argv[1]) && argv[2] == NULL) {
      parallelForm = "pthreads";
      noOfThreads = argv[1];
    } else {
      parallelForm = "pthreads";
      noOfThreads = sysconf(_SC_NPROCESSORS_ONLN);
    }

    const uint16_t kMaxCharacters = 999;
    char input[kMaxCharacters];

    // Managing Input
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
    if (noOfThreads > 1) {
      if (!strcmp(parallelForm, "pthread")) {
        printf("PTHREADS Selected\n");
        zippass_pthread(lines, noOfThreads, fileCount, lineCount);
      } else if (!strcmp(parallelForm, "omp")) {
        printf("OMP Selected\n");
        zippass_omp(lines, noOfThreads, fileCount, lineCount);
      } else {
        fprintf(stderr, "Enter a valid parallel mode\n");
      }
    } else {
      zippass_serial(lines, fileCount, lineCount);
    }
    free(parallelForm);
  } else {
    // ENTER HERE THE TESTS YOU WANT TO RUN
    // passwordGenTimer();
    // decryptTesterTimerWrong();
    // decryptTesterTimerRight();
    // canFileBeOpenedMultipleTimes();
    enqueueDequeueTesting();
  }
}
