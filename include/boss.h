#ifndef BOSS_H
#define BOSS_H

#include "gemini_ai.h"
#include "player.h"

void Boss_Spawn(DynamicEnemyStats stats);

void Boss_Update(Player* player);

#endif 