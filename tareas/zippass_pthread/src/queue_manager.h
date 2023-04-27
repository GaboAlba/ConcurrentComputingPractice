

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

char** createQueue(uint64_t QueueSize);

char* dequeue(char** Queue);

void enqueue(char** Queue, char* password);


#endif
