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

/// @brief Struct that defines the data that can be accessed by any file and/or
///        thread
typedef struct publicData {
    char* alphabet;
    uint8_t sizeOfAlphabet;
    uint8_t maxPwdLength;
    uint8_t threadCount;
} publicData_t;

typedef struct FilesData {
    char* filePath;
    char* password;
    bool passwordFound;
} FilesData_t;

typedef struct QueueData {
    char** Queue;
    uint8_t QueueId;
} QueueData_t;

/// @brief Struct that defines the data that is property of the file and/or
///        thread and may not be shared among them
typedef struct threadData {
    uint8_t threadNumber;
    QueueData_t* QueueData;
    FilesData_t* FilesData;
    publicData_t* publicData;
} threadData_t;