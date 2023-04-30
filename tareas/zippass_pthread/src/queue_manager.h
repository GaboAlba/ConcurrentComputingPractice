

#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>

#include "data_structures.h"

QueueData_t* createQueue(QueueData_t* QueueData, uint8_t QueueId);

char* dequeue(QueueData_t* QueueData);

void enqueue(QueueData_t* QueueData, char* password);

int isQueueFull(QueueData_t* QueueData);

int isQueueEmpty(QueueData_t* QueueData);


#endif
