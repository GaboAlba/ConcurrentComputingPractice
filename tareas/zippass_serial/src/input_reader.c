// Copyright yman@StackOverflow and Gabriel Alba <gabriel.alba@ucr.ac.cr>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>

#include "input_reader.h"

// Taken from https://stackoverflow.com/questions/17983005/c-how-to-read-a-string-line-by-line
int countChar(char* str, char c) {
  char* nextChar = strchr(str, c);
  int count = 0;

  while (nextChar) {
    count++;
    nextChar = strchr(nextChar + 1, c);
  }

  return count;
}

// Taken from https://stackoverflow.com/questions/17983005/c-how-to-read-a-string-line-by-line
char** lineator(char* origin) {
  char* str = (char*) malloc(strlen(origin) + 1);
  strcpy(str, origin);

  int count = countChar(origin, '\n');
  char** lines = (char**) malloc(sizeof(char *) * count);

  char* nextLine = strchr(str, '\n');
  char* currentLine = str;

  int i = 0;

  while (nextLine) {
    *nextLine = '\0';

    lines[i] = malloc(strlen(currentLine) + 1);
    strcpy(lines[i], currentLine);

    currentLine = nextLine + 1;
    nextLine = strchr(currentLine, '\n');

    i++;
  }

  free(str);
  return lines;
}

// PENDING
int validateAlphabet(char* alphabet) {
  char uniqueChars[strlen(alphabet)];
  char tempChar = '\0';
  uniqueChars[0] = alphabet[0];
  for (uint8_t character = 1; character < strlen(alphabet); character++) {
    printf("\nAL_CHAR: %c", alphabet[character]);
    for (uint8_t unique = 1; unique < strlen(alphabet); unique++) {
      printf(" UN_CHAR: %c", uniqueChars[unique]);
      tempChar = alphabet[character];
      if (alphabet[character] == uniqueChars[unique]) {
        return EXIT_FAILURE;
      } else {
        strcat(uniqueChars, &tempChar);
        //printf("%s\n", uniqueChars);
      }
    }
  }
  return EXIT_SUCCESS;
}

// VALIDATED
int validateLength(char* length) {
  for (uint8_t character = 0; character < strlen(length); character++) {
    char charToAn = length[character];
    if (!strcmp(&charToAn,"0") || !strcmp(&charToAn,"1") || !strcmp(&charToAn,"2") || 
        !strcmp(&charToAn,"3") || !strcmp(&charToAn,"4") || !strcmp(&charToAn,"5") ||
        !strcmp(&charToAn,"6") || !strcmp(&charToAn,"7") || !strcmp(&charToAn,"8") ||
        !strcmp(&charToAn,"9")) {
        } else {
          fprintf(stderr, "ERROR: You cannot have non-number symbols in your length\n");
          return EXIT_FAILURE;
        }
  }
  return EXIT_SUCCESS;
}

// VALIDATED
int validateEmptyLine(char* emptyLine) {
  if (*emptyLine != '\0' && *emptyLine != ' ') {
    fprintf(stderr, "ERROR: A blank line is expected between Length and Files\n");
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }

}

// VALIDATED
int validateFileFormat(char* file) {
  regex_t regex;
  int8_t error;
  error = regcomp(&regex, "^/?([0-9a-zA-Z_]+/?)*[0-9A-Za-z_]+\\.zip", REG_EXTENDED);
  if (error != 0) {
    fprintf(stderr, "ERROR: Regex not compiled\n");
  } else {
      error = regexec(&regex, file, 0, NULL, 0);
      if (error == REG_NOMATCH) {
        fprintf(stderr, "ERROR: File **%s** doesn't have the correct format\n", file);
        regfree(&regex);
        return EXIT_FAILURE;
      }
    }
  regfree(&regex);
  return EXIT_SUCCESS;
}

// VALIDATED
int validateFileExists(char* file) {
  
  int error;
  // Open ZIP file
  zip_t* testFile = zip_open(file, ZIP_RDONLY, &error);
  if (testFile == NULL) {
      fprintf(stderr, "ERROR: File **%s** does not exist", file);
      zip_discard(testFile);
      return EXIT_FAILURE;
  } else {
    zip_discard(testFile);
    return EXIT_SUCCESS;
  }
  
}

