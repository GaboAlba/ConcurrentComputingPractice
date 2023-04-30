
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>

#include "data_structures.h"

QueueData_t* createQueue(QueueData_t* QueueData, uint8_t QueueId){
    QueueData->Queue = calloc(QueueData->QueueMaxSize, sizeof(char*));
    QueueData->QueueCurrentSize = 0;
    QueueData->front = -1;
    QueueData->rear = -1;
    QueueData->QueueId = QueueId;
    return QueueData; 
}

char* dequeue(QueueData_t* QueueData) {
    char* password = QueueData->Queue[QueueData->front++];
    if(QueueData->front == QueueData->QueueMaxSize) {
        QueueData->front = 0;
    }
	
    QueueData->QueueCurrentSize--;
    return password;
}

void enqueue(QueueData_t* QueueData, char* password) {
    if(isQueueEmpty(QueueData)) {
        QueueData->front = 0;
    }
    if (!isQueueFull(QueueData)) {
        QueueData->rear = (QueueData->rear + 1) % QueueData->QueueMaxSize;
        QueueData->Queue[QueueData->rear] = calloc(strlen(password) + 1, sizeof(char));
        strcpy(QueueData->Queue[QueueData->rear], password);
        QueueData->QueueCurrentSize++;
    }
}

int isQueueFull(QueueData_t* QueueData) {
    return QueueData->QueueCurrentSize == QueueData->QueueMaxSize;
}

int isQueueEmpty(QueueData_t* QueueData) {
    return QueueData->QueueCurrentSize == 0;
}