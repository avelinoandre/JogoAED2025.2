#include "score.h"
#include "raylib.h" 
#include <stdio.h>   

static int playerScore;
static int finalScore;
static float gameTimer;
static bool isPlayerDead;
static bool scoreCalculated;
static const char* rankText;

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
        finalScore = (int)(((float)playerScore * 100.0f) / gameTimer);
    }
    
    if (finalScore > 750) rankText = "RANK: S (SMASHING!)";
    else if (finalScore > 500) rankText = "RANK: A (AWESOME)";
    else if (finalScore > 250) rankText = "RANK: B (COOL)";
    else if (finalScore > 100) rankText = "RANK: C (OKAY)";
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