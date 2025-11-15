#ifndef CAIXA_H
#define CAIXA_H

#include "raylib.h"
#include "player.h"

void Caixa_Init(void);
void Caixa_Unload(void);
void Caixa_Update(Player *player);
void Caixa_Draw(void);
void Caixa_DespawnAll(void);
void Caixa_Spawn_At(Vector2 position);

#endif