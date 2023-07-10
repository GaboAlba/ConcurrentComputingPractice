// Copyright yman@StackOverflow and Gabriel Alba <gabriel.alba@ucr.ac.cr>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>
#include <sys/stat.h>

#include "input_reader.h"

// Taken from https://stackoverflow.com/questions/17983005/c-how-to-read-a-string-line-by-line
//*****************************************************************************
/// @brief Reads a string and determines the number of lines in it
/// @param str String containing the information related to the zip files to be
///            decrypted
/// @param c Char that defines a newline
/// @return Count of lines
///     @retval count
//*****************************************************************************
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
//*****************************************************************************
/// @brief Reads a string and creates an array of the lines of it
/// @param origin String containing the information related to the zip files to
///               be decrypted
/// @return An error code:
///     @retval lines: Array of all lines that were contained in **origin**
//*****************************************************************************
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

// Taken from http://www.java2s.com/Tutorial/Cpp/0100__Development/ArecursiveversionofQuicksortforsortingcharacters.htm
//*****************************************************************************
/// @brief Reads a string and it's length and sorts it
/// @param items String, in this case used for the alphabet
/// @param len Length of the string to be sorted
//*****************************************************************************
void quickSort(char *items, int len) {
  qs(items, 0, len-1);
}

// Taken from http://www.java2s.com/Tutorial/Cpp/0100__Development/ArecursiveversionofQuicksortforsortingcharacters.htm
//*****************************************************************************
/// @brief Reads a string and it's most left and most right chars and sorts the
///        string in ascendent order
/// @param items String, in this case used for the alphabet
/// @param left Most left element on the string
/// @param right Most right element on the string
//*****************************************************************************
void qs(char *items, int left, int right) {
  int i, j;
  char x, y;

  i = left; j = right;
  x = items[(left+right) / 2 ];

  do {
    while ((items[i] < x) && (i < right))
       i++;
    while ((x < items[j]) && (j > left))
       j--;

    if (i <= j) {
      y = items[i];
      items[i] = items[j];
      items[j] = y;
      i++; j--;
    }
  } while (i <= j);

  if (left < j)
     qs(items, left, j);
  if (i < right)
     qs(items, i, right);
}

//*****************************************************************************
/// @brief Reads the **alphabet** provided and validates that it has no
//         duplicate characters
/// @param alphabet String containing all possible values for the password
/// @return An error code:
///     @retval 0 for success
///     @retval 1 for error
//*****************************************************************************
int validateAlphabet(const char* alphabet) {
  quickSort(alphabet, strlen(alphabet));
  char currentValue = alphabet[0];
  char nextValue = alphabet[1];
  for (uint8_t element = 0; element < strlen(alphabet) - 1; element++) {
    if (currentValue == nextValue) {
      fprintf(stderr, "ERROR: Please ensure your alphabet has no duplicates\n");
      return EXIT_FAILURE;
    } else {
      currentValue = nextValue;
      nextValue = alphabet[element + 2];
    }
  }
  return EXIT_SUCCESS;
}

//*****************************************************************************
/// @brief Reads the **length** provided in the input and verifies it only
///        contains numbers
/// @param length String containing the maximum length of the password
/// @return An error code:
///     @retval 0 for success
///     @retval 1 for error
//*****************************************************************************
int validateLength(char* length) {
  for (uint8_t character = 0; character < strlen(length); character++) {
    char charToAn = length[character];
    if (!strcmp(&charToAn, "0") || !strcmp(&charToAn, "1") ||
        !strcmp(&charToAn, "2") || !strcmp(&charToAn, "3") ||
        !strcmp(&charToAn, "4") || !strcmp(&charToAn, "5") ||
        !strcmp(&charToAn, "6") || !strcmp(&charToAn, "7") ||
        !strcmp(&charToAn, "8") || !strcmp(&charToAn, "9")) {
        } else {
          fprintf(stderr,
                "ERROR: You cannot have non-number symbols in your length\n");
          return EXIT_FAILURE;
        }
  }
  return EXIT_SUCCESS;
}

//*****************************************************************************
/// @brief Reads the third line in the input and validates that it is empty
/// @param emptyLine String that should be empty
/// @return An error code:
///     @retval 0 for success
///     @retval 1 for error
//*****************************************************************************
int validateEmptyLine(char* emptyLine) {
  if (*emptyLine != '\0' && *emptyLine != ' ') {
    fprintf(stderr,
            "ERROR: A blank line is expected between Length and Files\n");
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}

//*****************************************************************************
/// @brief Reads a file_path string and validates that it has the correct
///        format to be read as a path
/// @param file String containing the path to a zip file to be decrypted
/// @return An error code:
///     @retval 0 for success
///     @retval 1 for error
//*****************************************************************************
int validateFileFormat(char* file) {
  regex_t regex;
  int8_t error;
  error = regcomp(&regex,
                  "^/?([0-9a-zA-Z_]+/?)*[0-9A-Za-z_]+\\.zip",
                  REG_EXTENDED);
  if (error != 0) {
    fprintf(stderr, "ERROR: Regex not compiled\n");
  } else {
      error = regexec(&regex, file, 0, NULL, 0);
      if (error == REG_NOMATCH) {
        fprintf(stderr,
                "ERROR: File **%s** doesn't have the correct format\n",
                file);
        regfree(&regex);
        return EXIT_FAILURE;
      }
    }
  regfree(&regex);
  return EXIT_SUCCESS;
}

//*****************************************************************************
/// @brief Reads a file_path string and validates that it points to an existing
///        file that can be opened
/// @param file String containing the path to a zip file to be decrypted
/// @return An error code:
///     @retval 0 for success
///     @retval 1 for error
//*****************************************************************************
int validateFileExists(char* file) {
  int error;
  // Open ZIP file
  zip_t* testFile = zip_open(file, ZIP_RDONLY, &error);
  if (testFile == NULL) {
      fprintf(stderr, "ERROR: File **%s** does not exist\n", file);
      zip_discard(testFile);
      return EXIT_FAILURE;
  } else {
    zip_discard(testFile);
    return EXIT_SUCCESS;
  }
}

//*****************************************************************************
/// @brief Reads a string and determines if it's only composed of ASCII
///        characters
/// @param input String containing the information related to the zip files to
///              be decrypted
/// @return An error code:
///     @retval 0 for success
///     @retval 1 for **input** has at least one character not ASCII
//*****************************************************************************
int validateASCIIInput(char input[]) {
  uint64_t inputSize = strlen(input);
  for (uint32_t character = 0; character < inputSize; character++) {
    int error = __isascii(input[character]);
    if (error) {
    } else {
      fprintf(stderr, "ERROR: Character %c is not ASCII\n",
        input[character]);
      return 1;
    }
  }
  return 0;
}

//*****************************************************************************
/// @brief Validates that the input provided is valid and that it won't create
///        unnecesary load to the system
/// @param lines Array of all the lines of the input
/// @param lineCount The number of lines of the input
/// @return An error code:
///     @retval 0 for success
///     @retval 1 for any error
//*****************************************************************************
int validateInputFormat(char** lines, uint8_t lineCount) {
  uint8_t formatValid = 0;

  // Validate alphabet has no duplicates
  int8_t error = validateAlphabet((const char*) lines[0]);
  if (error == EXIT_FAILURE) {
    formatValid = 1;
  }
  // Validate Length value
  error = validateLength(lines[1]);
  if (error == EXIT_FAILURE) {
    formatValid = 1;
  }
  // Validate the empty line
  error = validateEmptyLine(lines[2]);
  if (error == EXIT_FAILURE) {
    formatValid = 1;
  }
  // validate file path format is correct
  for (uint8_t file = 3; file < lineCount; file++) {
    error = validateFileFormat(lines[file]);
    if (error == EXIT_FAILURE) {
        formatValid = 1;
    }
    error = validateFileExists(lines[file]);
    if (error == EXIT_FAILURE) {
        formatValid = 1;
    }
  }

  return formatValid;
}


