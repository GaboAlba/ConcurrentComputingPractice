// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>
///  @file
///  @brief Main source program

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


//**********************************************************************************************************************
///    @brief Asks the user for the input and validates it. After this it calls
///           the subroutine to decipher the password.
///    @param void Receives no parameters
///    @return An error code:
///        @retval 0 for success
///        @retval int for any error
//**********************************************************************************************************************
int main(void) {
  if (!DEBUGCONST) {
    uint8_t noOfThreads = 2;
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
      zippass_pthread(lines, noOfThreads, fileCount, lineCount);
    } else {
      zippass_serial(lines, fileCount, lineCount);
    }
    return 0;
  } else {
    // ENTER HERE THE TESTS YOU WANT TO RUN 
    //passwordGenTimer();
    //decryptTesterTimerWrong();
    //decryptTesterTimerRight();
    //canFileBeOpenedMultipleTimes();
    enqueueDequeueTesting();

  }
}
