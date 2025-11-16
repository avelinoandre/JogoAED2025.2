#ifndef BOSS_H
#define BOSS_H

#include "gemini_ai.h"
#include "player.h"
#include <stdbool.h>

void Boss_Init(void);

void Boss_Spawn(DynamicEnemyStats stats);

void Boss_Update(Player* player);

bool Boss_IsDefeated(void);

bool Boss_IsShielded(void);

#endif 