#include "score.h"
#include "raylib.h" 
#include <stdio.h>   

static int playerScore;
static int finalScore;
static float gameTimer;
static bool isPlayerDead;
static bool scoreCalculated;
static const char* rankText;

static const char *SCORE_FILENAME = "highscores.txt";

void Score_Init(void) {
    playerScore = 0;
    finalScore = 0;
    gameTimer = 0.0f;
    isPlayerDead = false;
    scoreCalculated = false;
    rankText = "";
}

void Score_Update(float deltaTime) {
    if (!isPlayerDead) {
        gameTimer += deltaTime;
    }
}

void Score_AddPoints(int points) {
    if (!isPlayerDead) {
        playerScore += points;
    }
}

void Score_SetPlayerDead(bool dead) {
    isPlayerDead = dead;
}

bool Score_IsPlayerDead(void) {
    return isPlayerDead;
}

void Score_CalculateFinal(void) {
    if (scoreCalculated) return; 

    if (gameTimer < 1.0f) { 
        finalScore = 0;
    } else {
        finalScore = (int)(((float)playerScore * 1000) / gameTimer);
    }
    
    if (finalScore > 4500) rankText = "RANK: S (SMASHING!)";
    else if (finalScore > 3000) rankText = "RANK: A (AWESOME)";
    else if (finalScore > 1500) rankText = "RANK: B (COOL)";
    else if (finalScore > 500) rankText = "RANK: C (OKAY)";
    else rankText = "RANK: D (TRY AGAIN)";

    scoreCalculated = true;
}

int Score_GetScore(void) {
    return playerScore;
}

int Score_GetFinalScore(void) {
    return finalScore;
}

float Score_GetTimer(void) {
    return gameTimer;
}

const char* Score_GetRank(void) {
    return rankText;
}

void Score_SaveFinalScore(int score, int mode, const char* name) {
    FILE *file = fopen(SCORE_FILENAME, "a");
    if (file == NULL) {
        TraceLog(LOG_ERROR, "Nao foi possivel abrir o arquivo de scores para salvar.");
        return;
    }

    fprintf(file, "%s %d %d\n", name, mode, score);
    
    fclose(file);
}
