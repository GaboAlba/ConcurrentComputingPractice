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
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    // Define variables
    uint8_t noOfThreads;
    uint8_t lineCount;
    uint8_t fileCount;
    uint8_t permFileCount;
    struct timespec start_time, finish_time;
    char** lines;
    char* parallelForm = calloc(8, sizeof(char));
    char* file = malloc(sizeof(char) * 999);  // Dynamically allocated array for file
    char* alphabet = calloc(128, sizeof(char));
    char* maxPwdLength = calloc(128, sizeof(char));
    char* password = calloc(128, sizeof(char));
    char** files = calloc(128, sizeof(char*));
    char** passwords = calloc(128, sizeof(char*));
    int8_t error;
    MPI_Comm MPI_COMM_IDLE;
    

    // Initialize MPI
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    // Define functionality for process 0
    if (process_number == 0) {
      if (!DEBUGCONST) {
        if (argv[1] != NULL && argv[2] != NULL) {
          strcpy(parallelForm, argv[1]);
          noOfThreads = strtoull(argv[2], NULL, 10);
        } else {
          strcpy(parallelForm, "mpi");
          noOfThreads = sysconf(_SC_NPROCESSORS_ONLN);
        }

        //printf("Got parallel form %s\n", parallelForm);

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
        permFileCount = fileCount;
        //printf("permFileCount: %i", permFileCount);
        lines = lineator(input);
        error = validateInputFormat(lines, lineCount);
        if (error) {
          printf("ERROR: Invalid Format");
          return error;
        }
        if (noOfThreads > 1) {
          //printf("Running in parallel mode\n");
          if (!strcmp(parallelForm, "pthreads")) {
            printf("PTHREADS Selected\n");
            zippass_pthread(lines, noOfThreads, fileCount, lineCount);
          } else if (!strcmp(parallelForm, "omp")) {
            printf("OMP Selected\n");
            zippass_omp(lines, noOfThreads, fileCount, lineCount);
          } else if (!strcmp(parallelForm, "mpi")){
            //printf("MPI Selected\n");
            
            // Start Timer
            clock_gettime(CLOCK_MONOTONIC, &start_time);

            // Send permFileCount to all processes
            for (int target = 1; target < process_count; target++) {
              if (MPI_Send(&permFileCount, 1, MPI_UINT8_T, target,
                            0, MPI_COMM_WORLD) != MPI_SUCCESS) {
                  fprintf(stderr, "Could not send value to process %i\n", target);
              }
            }
            for (int target = 1; target <= permFileCount; target++) {
              if (target <= permFileCount) {
                strcpy(file, lines[2 + target]);  // Copy the file contents
                if (MPI_Send(file, strlen(file) + 1, MPI_CHAR, target,
                            0, MPI_COMM_WORLD) != MPI_SUCCESS) {
                  fprintf(stderr, "Could not send value to process %i\n", target);
                }
                //printf("Process 0 Sent File to process %i\n", target);
                strcpy(alphabet, lines[0]);  // Copy the file contents
                if (MPI_Send(alphabet, strlen(alphabet) + 1, MPI_CHAR, target,
                            0, MPI_COMM_WORLD) != MPI_SUCCESS) {
                  fprintf(stderr, "Could not send value to process %i\n", target);
                }
                //printf("Process 0 Sent Alphabet to process %i\n", target);
                strcpy(maxPwdLength, lines[1]);
                if (MPI_Send(maxPwdLength, strlen(maxPwdLength) + 1, MPI_CHAR, target,
                            0, MPI_COMM_WORLD) != MPI_SUCCESS) {
                  fprintf(stderr, "Could not send value to process %i\n", target);
                }
                //printf("Process 0 Sent maxPwdLength to process %i\n", target);
                if (MPI_Send(&noOfThreads, 1, MPI_UINT8_T, target,
                            0, MPI_COMM_WORLD) != MPI_SUCCESS) {
                  fprintf(stderr, "Could not send noOfThreads to process %i\n", target);
                }
                //printf("Process 0 Sent noOfThreads to process %i\n", target);
                int tempFileCount = fileCount;
                fileCount = 1;
                if (MPI_Send(&fileCount, 1, MPI_UINT8_T, target,
                            0, MPI_COMM_WORLD) != MPI_SUCCESS) {
                  fprintf(stderr, "Could not send fileCount to process %i\n", target);
                }
                fileCount = tempFileCount;
                //printf("Process 0 Sent fileCount to process %i\n", target);
                lineCount = 4;
                if (MPI_Send(&lineCount, 1, MPI_UINT8_T, target,
                            0, MPI_COMM_WORLD) != MPI_SUCCESS) {
                  fprintf(stderr, "Could not send lineCount to process %i\n", target);
                }
                //printf("Process 0 Sent lineCount to process %i\n", target);
              }
            }
            MPI_Barrier(MPI_COMM_WORLD);
          } else {
            fprintf(stderr, "Enter a valid parallel mode\n");
          }
        } else {
          printf("Single Threaded Mode Engaged\n");
          zippass_serial(lines, fileCount, lineCount);
        }
        for (int target = 1; target <= permFileCount; target++) {
          char* received_file = calloc(999, sizeof(char));
          if (MPI_Recv(received_file, 999, MPI_CHAR, target,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
            fprintf(stderr, "Could not receive file on process %i\n", process_number);
          }
          //printf("Process 0: Received file %s on process 0 from target %d\n", received_file, target);
          files[target - 1] = received_file;

          char* received_password = calloc(999, sizeof(char));
          if (MPI_Recv(received_password, 999, MPI_CHAR, target,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
            fprintf(stderr, "Could not receive file on process %i\n", process_number);
          }
          //printf("Process 0: Received password %s on process 0 from target %d\n", received_password, target);
          passwords[target - 1] = received_password;
          // printf("%i of %i", target, permFileCount);
        }

        for (int8_t spaces = 0; spaces < 5; spaces++) {
          printf("\n");
        }
          // Stop taking time
        clock_gettime(CLOCK_MONOTONIC, &finish_time);
        double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
                              (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("---Decrypting your password(s) with %" PRIu8 " threads with MPI---\n",
        noOfThreads);
        for (int index = 0; index < permFileCount; index++) {
          printf("%s %s\n", files[index], passwords[index]);
        }
        printf("---Password(s) decrypted---\n");
        printf("Time: %.9lfs\n", elapsed_time);
        //MPI_Barrier(MPI_COMM_WORLD);
      } else {
        // ENTER HERE THE TESTS YOU WANT TO RUN
        // passwordGenTimer();
        // decryptTesterTimerWrong();
        // decryptTesterTimerRight();
        // canFileBeOpenedMultipleTimes();
        enqueueDequeueTesting();
      }
    } else {
      MPI_Barrier(MPI_COMM_WORLD);
      if (MPI_Recv(&permFileCount, 1, MPI_UINT8_T, 0,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          fprintf(stderr, "Could not receive file on process %i\n", process_number);
      }
      if (process_number <= permFileCount) {
        //printf("Process %i receiving information\n", process_number);
        if (MPI_Recv(file, 999, MPI_CHAR, 0,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          fprintf(stderr, "Could not receive file on process %i\n", process_number);
        }
        if (MPI_Recv(alphabet, 128, MPI_CHAR, 0,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          fprintf(stderr, "Could not receive file on process %i\n", process_number);
        }
        if (MPI_Recv(maxPwdLength, 128, MPI_CHAR, 0,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          fprintf(stderr, "Could not receive file on process %i\n", process_number);
        }
        if (MPI_Recv(&noOfThreads, 1, MPI_UINT8_T, 0,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          fprintf(stderr, "Could not receive noOfThreads on process %i\n", process_number);
        }
        if (MPI_Recv(&fileCount, 1, MPI_UINT8_T, 0,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          fprintf(stderr, "Could not receive fileCount on process %i\n", process_number);
        }
        if (MPI_Recv(&lineCount, 1, MPI_UINT8_T, 0,
                      0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
          fprintf(stderr, "Could not receive lineCount on process %i\n", process_number);
        }
        zippass_mpi(alphabet, maxPwdLength, file, noOfThreads, fileCount, lineCount);
      } else {
        printf("");
      }
    } 
    free(parallelForm);
    free(file);
    free(alphabet);
    free(maxPwdLength);
    free(password);
    for (int index = 0; index < permFileCount; index++) {
      free(files[index]);
      free(passwords[index]);
    }
    free(files);
    free(passwords);
    MPI_Barrier(MPI_COMM_WORLD);
  } else {
    fprintf(stderr, "Could not init MPI");
  }
  MPI_Finalize();
}
