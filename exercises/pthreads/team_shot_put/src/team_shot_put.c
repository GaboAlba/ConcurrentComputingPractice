// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

void* team_throws(void* data);

typedef struct team_shot_put {
    uint64_t numOfPlayers;
    uint64_t numofThrows;
} team_shot_put_type;


typedef struct privateData {
    uint64_t teamNumber;
    uint64_t* teamScores;
    team_shot_put_type* publicData;

} privateData_type;


// procedure main:
int main(void) {

    srand(time(NULL));
    uint64_t numPlayersByTeam = 8;
    uint64_t numOfShotsByTeam = 3;
    privateData_type* teamOneData = (privateData_type*) calloc(1, sizeof(privateData_type));
    privateData_type* teamTwoData = (privateData_type*) calloc(1, sizeof(privateData_type));
    team_shot_put_type* publicData = (team_shot_put_type*) calloc(1, sizeof(team_shot_put_type));
    pthread_t threadOne;
    pthread_t threadTwo;
    publicData->numOfPlayers = numPlayersByTeam;
    publicData->numofThrows = numOfShotsByTeam;
    teamOneData->teamNumber = 1;
    teamTwoData->teamNumber = 2;

    int error1 = pthread_create(&threadOne, NULL, team_throws, &teamOneData);
    int error2 = pthread_create(&threadTwo, NULL, team_throws, &teamTwoData);
    printf("Thread created\n");

    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);

    free(teamOneData);
    free(teamTwoData);
    return EXIT_SUCCESS;
}
// end procedure

void* team_throws(void* teamData) {
    privateData_type* privateData = teamData;
    team_shot_put_type* publicData = privateData->publicData;
    printf(publicData->numOfPlayers);
    printf(publicData->numofThrows);
    uint64_t* teamScores = calloc(publicData->numOfPlayers,sizeof(uint64_t));
    for (uint64_t player = 0; player < publicData->numOfPlayers;) {
        uint64_t playerScore = 0;
        for (uint64_t throw = 0; throw < publicData->numofThrows; throw++) {
            uint64_t newPlayerScore = rand(); // Make it from 0 to 25
            if (newPlayerScore > playerScore) {
                playerScore = newPlayerScore;
            }
        }
        printf(player);
        printf(", ");
        printf(playerScore);
        printf("\n");
        teamScores[player] = playerScore;
    }

    privateData->teamScores = teamScores;
    free(teamScores);
    return privateData->teamScores;
}

