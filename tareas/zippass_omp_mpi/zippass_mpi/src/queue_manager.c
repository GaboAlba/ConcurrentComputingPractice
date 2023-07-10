// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>

#include "queue_manager.h"
#include "data_structures.h"

//***************************************************************************
/// @brief Takes allocated QueueData and initializes the attributes for the
///        queue
///
/// @param QueueData  QueueData type that contains all the data related to a
///                   specific queue
/// @param QueueId    uint8 type that numbers the specific queue.
///
/// @return The updated QueueData
///    @retval QueueData containing all data initilized for the given queue
//***************************************************************************
QueueData_t* createQueue(QueueData_t* QueueData, uint8_t QueueId) {
    QueueData->Queue = calloc(QueueData->QueueMaxSize, sizeof(char*));
    QueueData->QueueCurrentSize = 0;
    QueueData->front = -1;
    QueueData->rear = -1;
    QueueData->QueueId = QueueId;
    return QueueData;
}

//***************************************************************************
/// @brief Grabs the front value of the queue, returns it and moves the front
///        one space forwards
///
/// @param QueueData  Data containing all current data of the queue like front,
///                   rear, size etc.
///
/// @return The string of the password that may unlock the file in the zip file
//***************************************************************************
char* dequeue(QueueData_t* QueueData) {
    char* password = QueueData->Queue[QueueData->front++];
    if (QueueData->front == QueueData->QueueMaxSize) {
        QueueData->front = 0;
    }
    QueueData->QueueCurrentSize--;
    return password;
}

//***************************************************************************
/// @brief Pushes a new value into the back of the queue
///
/// @param QueueData  Data containing all current data of the queue like front,
///                   rear, size etc.
/// @param password   String that will be pushed into the queue
///
//***************************************************************************
void enqueue(QueueData_t* QueueData, char* password) {
  if (isQueueEmpty(QueueData)) {
      QueueData->front = 0;
  }
  if (!isQueueFull(QueueData)) {
    QueueData->rear = (QueueData->rear + 1) % QueueData->QueueMaxSize;
    QueueData->Queue[QueueData->rear] = calloc(strlen(password) + 1,
                                               sizeof(char));
    strcpy(QueueData->Queue[QueueData->rear], password);
    QueueData->QueueCurrentSize++;
  }
}

//***************************************************************************
/// @brief Verifies if the queue is full or not
///
/// @param QueueData  Data containing all current data of the queue like front,
///                   rear, size etc.
/// @return Bool
///  @retval 1 if the queue is full
///  @retval 0 if the queue is NOT full
//***************************************************************************
int isQueueFull(QueueData_t* QueueData) {
    return QueueData->QueueCurrentSize == QueueData->QueueMaxSize;
}

//***************************************************************************
/// @brief Verifies if the queue is empty or not
///
/// @param QueueData  Data containing all current data of the queue like front,
///                   rear, size etc.
/// @return Bool
///  @retval 1 if the queue is empty
///  @retval 0 if the queue is NOT empty
//***************************************************************************
int isQueueEmpty(QueueData_t* QueueData) {
    return QueueData->QueueCurrentSize == 0;
}
