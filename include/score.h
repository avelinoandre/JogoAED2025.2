#ifndef SCORE_H
#define SCORE_H

#include "raylib.h"
#include <stdbool.h>

void Score_Init(void);

void Score_Update(float deltaTime);

void Score_AddPoints(int points);

void Score_SetPlayerDead(bool dead);

void Score_CalculateFinal(void);


int Score_GetScore(void);          
int Score_GetFinalScore(void);      
float Score_GetTimer(void);        
const char* Score_GetRank(void);    
bool Score_IsPlayerDead(void);      

void Score_SaveFinalScore(int score, int mode, const char* name);
int Score_LoadHighScores(int *scoresArray, int maxScores);
void Score_SortHighScores(int *scoresArray, int count);

#endif