#ifndef CAIXA_H
#define CAIXA_H

#include "raylib.h"
#include "player.h"

void Caixa_Init(void);
void Caixa_Unload(void);
// MODIFICADO: Agora aceita ambos os jogadores e o estado de jogo
void Caixa_Update(Player *player1, Player *player2, bool isPlayer2Active);
void Caixa_Draw(void);
void Caixa_DespawnAll(void);
void Caixa_Spawn_At(Vector2 position);

#endif