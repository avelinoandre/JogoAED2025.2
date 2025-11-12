#ifndef ALGORITMO_INIMIGOS_H
#define ALGORITMO_INIMIGOS_H

#include "player.h"
#include "mapa.h"

void ControleSpawn_IniciaCena(SceneNode* cena, Player* jogador);
void ControleSpawn_Update(float deltaTime, Player* jogador);
int ControleSpawn_GetTotalInimigos(void);
bool ControleSpawn_EstaAtivo(void);
int ControleSpawn_GetInimigosSpawnados(void); 

#endif