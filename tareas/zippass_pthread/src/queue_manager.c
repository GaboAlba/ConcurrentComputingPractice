
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>

#include "data_structures.h"

void createQueue(QueueData_t QueueData){
    QueueData.Queue = calloc(QueueData.QueueMaxSize, sizeof(char*));
}

char* dequeue(QueueData_t QueueData) {
    char* password = QueueData.Queue[QueueData.front++];
	
    if(QueueData.front == QueueData.QueueMaxSize) {
        QueueData.front = 0;
    }
	
    QueueData.QueueCurrentSize--;
    return password;
}

void enqueue(QueueData_t QueueData, char* password) {
    if (!isFull(QueueData)) {
        if(QueueData.rear == QueueData.QueueMaxSize - 1) {
          QueueData.rear = -1;            
        }       

      QueueData.Queue[++QueueData.rear] = password;
      QueueData.QueueCurrentSize++;
    }
}

bool isFull(QueueData_t QueueData) {
    return QueueData.QueueCurrentSize == QueueData.QueueMaxSize;
}

bool isEmpty(QueueData_t QueueData) {
    return QueueData.QueueCurrentSize == 0;
}