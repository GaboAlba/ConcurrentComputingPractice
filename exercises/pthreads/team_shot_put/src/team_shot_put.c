// Copyright [2023] Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

typedef struct playerData {
    uint64_t playerNumber;
    uint64_t team;
    double score;
} playerData_type;


void* team_throws(void* data);

// Taken from https://stackoverflow.com/questions/33058848/generate-a-random-double-between-1-and-1
/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

// procedure main:
int main(int argc, char* argv[]) {

    srand(time(NULL));
    int error = EXIT_SUCCESS;
    char* ptr;
    if (argv[1] == NULL) {
        fprintf(stderr, "ERROR -1: Input cannot be empty\n");
        error = -1;
        return error;
    }
    uint64_t numPlayersByTeam = strtol(argv[1], &ptr, 10);
    // Verify odd number of players
    if (numPlayersByTeam % 2 == 0) {
        fprintf(stderr, "ERROR 2: Number of players must be odd\n");
        error = 2;
        return error;
    } else {
        uint64_t numOfTeams = 2;
        pthread_t* thread = (pthread_t*) calloc((numOfTeams*numPlayersByTeam), sizeof(playerData_type));
        playerData_type* playerData = (playerData_type*) calloc((numOfTeams*numPlayersByTeam), sizeof(playerData_type));
        uint64_t id = 0;

        // Creating threads
        for (uint64_t team = 0; team < numOfTeams; team++) {
            for (uint64_t player = 0; player < numPlayersByTeam; player++) {
                if (team == 0){
                    id = player;
                } else if (team < numOfTeams && team > 0) {
                    id = (team * numPlayersByTeam) + player;
                } else if (team >= numOfTeams) {
                    fprintf(stderr, "ERROR 3: **team** cannot be larger than the number of teams\n");
                    error = 3;
                    return error;
                } else {
                    fprintf(stderr, "ERROR 4: **team** value is invalid\n");
                    error = 4;
                    return error;
                }
                playerData[id].team = team + 1;
                playerData[id].playerNumber = player + 1;
                playerData[id].score = 0;
                error = pthread_create(&thread[id], NULL, team_throws, &playerData[id]);
                if (error == EXIT_SUCCESS){
                } else {
                    fprintf(stderr,"ERROR: Could not create thread %" PRIu64 "\n", id);
                }
            }
            
        }

        // Making sure all thread finish and placing them in the right place
        for (id = 0; id < (numOfTeams * numPlayersByTeam); id++) {
            pthread_join(thread[id], (void*) &playerData[id]);
        }

        // Determining points by team
        uint64_t* pointsByTeam = (uint64_t*) calloc(numOfTeams, sizeof(uint64_t));
        double tempScore;
        double maxScore;
        playerData_type tempPlayerData;
        playerData_type maxPlayerData;
        for (uint64_t player = 0; player < numPlayersByTeam; player++) {
            tempScore = 0;
            maxScore = 0;
            for (uint64_t team = 0; team < numOfTeams; team++) {
                id = (team * numPlayersByTeam) + player;
                tempPlayerData = playerData[id];
                tempScore = tempPlayerData.score;
                if (tempScore > maxScore) {
                    maxScore = tempScore;
                    maxPlayerData = tempPlayerData;
                }
            }
            pointsByTeam[maxPlayerData.team - 1]++;
        }

        // Determining winner 
        uint64_t winningTeam = 0;
        printf("\n");
        for (uint64_t team = 0; team < numOfTeams; team++) {
            if (pointsByTeam[team] > pointsByTeam[winningTeam]) {
                winningTeam = team;
            }
            printf("%" PRIu64 ":", pointsByTeam[team]);
        }
        printf(", team %" PRIu64 " wins\n", winningTeam + 1);
        free(playerData);
        return EXIT_SUCCESS;
    }
}
// end procedure

void* team_throws(void* data) {
    uint64_t numOfThrows = 3;
    uint64_t minDistance = 0;
    uint64_t maxDistance = 25;
    playerData_type* playerData = (playerData_type*) data;
    double score; 
    for (uint64_t throw = 0; throw < numOfThrows; throw++) {
        double newPlayerScore = randfrom(minDistance,maxDistance);
        if (newPlayerScore > score) {
            score = newPlayerScore;
        }
    }

    playerData->score = score;
    printf("%" PRIu64 ".%" PRIu64 ": best shot put %fm\n",playerData->team, playerData->playerNumber, playerData->score);
    return playerData;
}

