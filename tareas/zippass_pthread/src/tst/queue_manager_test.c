
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

void enqueueDequeueTesting() {
    printf("-1\n");
    QueueData_t* QueueData = calloc(1, sizeof(QueueData_t));
    printf("0\n");
    QueueData->QueueMaxSize = 10;
    printf("1\n");
    uint8_t QueueId = 0;
    QueueData = createQueue(QueueData, QueueId);
    printf("2\n");
    printf("%i\n",isQueueEmpty(QueueData));
    char* password = calloc(5, sizeof(char));
    for (uint64_t num = 0; num < QueueData->QueueMaxSize; num++) {
        sprintf(password, "%d", num);
        enqueue(QueueData, password);
    }
    for (uint64_t num = QueueData->QueueMaxSize -1; num >= 0; num--){
        password = dequeue(QueueData);
        printf("%s\n", password);
    }
    
}