#ifndef CHAR_SELECT_H
#define CHAR_SELECT_H

#include "raylib.h"
#include "globals.h"

void InitCharSelectMenu(void);
void UpdateCharSelectMenu(int *gameState); 
void DrawCharSelectMenu(void);
void UnloadCharSelectMenu(void);

#endif