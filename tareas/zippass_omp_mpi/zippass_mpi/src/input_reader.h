// Copyright yman@StackOverflow
// Taken from https://stackoverflow.com/questions/17983005/c-how-to-read-a-string-line-by-line

#ifndef INPUT_READER_H
#define INPUT_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>

int countChar(char* str, char c);

char** lineator(char* origin);

int validateLength(char* length);

int validateASCIIInput(char input[]);

int validateInputFormat(char** lines, uint8_t lineCount);

#endif
