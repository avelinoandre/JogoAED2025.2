#ifndef ALGORITMO_INIMIGOS_H
#define ALGORITMO_INIMIGOS_H

#include "mapa.h"
#include "player.h"


void ControleSpawn_IniciaCena(SceneNode* cena, Player* player1, Player* player2, bool isPlayer2Active);
void ControleSpawn_Update(float deltaTime, Player* player1, Player* player2, bool isPlayer2Active);
int ControleSpawn_GetTotalInimigos();
bool ControleSpawn_EstaAtivo();
int ControleSpawn_GetInimigosSpawnados();

#endif