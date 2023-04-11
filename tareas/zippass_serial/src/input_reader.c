// Taken from https://stackoverflow.com/questions/17983005/c-how-to-read-a-string-line-by-line

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>

#include "input_reader.h"

int countChar(char* str, char c) {
  char* nextChar = strchr(str, c);
  int count = 0;

  while (nextChar) {
    count++;
    nextChar = strchr(nextChar + 1, c);
  }

  return count;
}

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