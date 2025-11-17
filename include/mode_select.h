#ifndef MODE_SELECT_H
#define MODE_SELECT_H

#include "raylib.h"
#include "globals.h"

void InitModeSelect(void);
int UpdateModeSelect(void); 
void DrawModeSelect(void);
void UnloadModeSelect(void);
GameMode GetGameMode(void); 

#endif 