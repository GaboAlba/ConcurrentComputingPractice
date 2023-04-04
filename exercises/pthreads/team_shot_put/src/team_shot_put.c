// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

uint64_t* team_throws(void* data);

typedef struct team_shot_put {
    uint64_t numOfPlayers;
    uint64_t numofThrows;
} team_shot_put;


typedef struct privateData {
    uint64_t teamNumber;
    uint64_t* teamScores;
    team_shot_put* publicData;

} privateData;


// procedure main:
int main(void) {
    uint64_t numPlayersByTeam = 8;
    uint64_t numOfShotsByTeam = 3;
    uint64_t numOfTeams = 2;
    team_shot_put publicData;
    privateData* teamData = calloc(numOfTeams, sizeof(privateData));
    teamData->publicData->numOfPlayers = numPlayersByTeam;
    teamData->publicData->numofThrows = numOfShotsByTeam;
    pthread_t* thread = calloc(2, sizeof(pthread_t*));
    for (uint64_t team = 0; team < numOfTeams; team++) {
        teamData->teamNumber = team + 1;
        int error = pthread_create(&thread[team], NULL, team_throws, &teamData[team]);
    }

    free(thread);
    free(teamData);
    return EXIT_SUCCESS;
}
// end procedure

uint64_t* team_throws(void* teamData) {
    privateData* privateData = teamData;
    team_shot_put* publicData = privateData->publicData;
    uint64_t* teamScores = calloc(publicData->numOfPlayers,sizeof(uint64_t));
    for (uint64_t player = 0; player < publicData->numOfPlayers;) {
        uint64_t playerScore = 0;
        for (uint64_t throw = 0; throw < publicData->numofThrows; throw++) {
            uint64_t newPlayerScore = rand(); // Make it from 0 to 25
            if (newPlayerScore > playerScore) {
                playerScore = newPlayerScore;
            }
        }
        teamScores[player] = playerScore;
    }

    return teamScores;
}

