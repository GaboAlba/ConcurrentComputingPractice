
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H


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
#include <semaphore.h>

/// @brief Struct that defines the data that can be accessed by any file and/or
///        thread
typedef struct publicData {
    char* alphabet;
    char** fileList;
    uint8_t sizeOfAlphabet;
    uint8_t maxPwdLength;
    uint8_t threadCount;
    uint8_t fileCount;
    uint8_t filesUnlocked;
    sem_t semaphore;
} publicData_t;

typedef struct FilesData {
    char* filePath;
    char* password;
    bool passwordFound;
} FilesData_t;

typedef struct QueueData {
    char** Queue;
    int64_t rear;
    int64_t front;
    uint64_t QueueMaxSize;
    uint64_t QueueCurrentSize;
    uint8_t QueueId;
} QueueData_t;

/// @brief Struct that defines the data that is property of the file and/or
///        thread and may not be shared among them
typedef struct testerThreadData {
    uint8_t threadNumber;
    bool threadDone;
    QueueData_t* QueueData;
    FilesData_t** FilesData;
    publicData_t* publicData;
} testerThreadData_t;

typedef struct pwdGenData {
    uint8_t threadNumber;
    QueueData_t** QueueData;
    publicData_t* publicData;
} pwdGenData_t;


#endif
