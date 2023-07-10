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
#include <mpi.h>

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
  // Define variables
  uint8_t noOfThreads;
  uint8_t lineCount;
  uint8_t fileCount;
  struct timespec start_time, finish_time;
  char** lines;
  char* parallelForm = calloc(8, sizeof(char));
  int8_t error;

  if (!DEBUGCONST) {
    if (argv[1] != NULL && argv[2] != NULL) {
      strcpy(parallelForm, argv[1]);
      noOfThreads = strtoull(argv[2], NULL, 10);
    } else {
      strcpy(parallelForm, "omp");
      noOfThreads = sysconf(_SC_NPROCESSORS_ONLN);
    }

    const uint16_t kMaxCharacters = 999;
    char input[kMaxCharacters];

    // Managing Input
    printf("Please enter required information (use ; when finished): \n");
    error = scanf("%[^;]s", &input);
    if (error > 0) {
    } else {
      fprintf(stderr, "ERROR: Could not read input\n");
      return error;
    }
    error = validateASCIIInput(input);
    if (error) {
      return error;
    }
    lineCount = countChar(input, '\n');
    fileCount = lineCount - 3;
    lines = lineator(input);
    error = validateInputFormat(lines, lineCount);
    if (error) {
      printf("ERROR: Invalid Format");
      return error;
    }
    if (noOfThreads > 1) {
      if (!strcmp(parallelForm, "pthreads")) {
        printf("PTHREADS Selected\n");
        zippass_pthread(lines, noOfThreads, fileCount, lineCount);
      } else if (!strcmp(parallelForm, "omp")) {
        printf("OMP Selected\n");
        zippass_omp(lines, noOfThreads, fileCount, lineCount);
      } else {
      } 
    } else {
      printf("Running in single threaded mode");
      zippass_serial(lines, fileCount, lineCount);
    }
  } else {
        // ENTER HERE THE TESTS YOU WANT TO RUN
        // passwordGenTimer();
        // decryptTesterTimerWrong();
        // decryptTesterTimerRight();
        // canFileBeOpenedMultipleTimes();
        enqueueDequeueTesting();
  }  
  free(parallelForm);
}
