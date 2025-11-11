#ifndef GEMINI_AI_H
#define GEMINI_AI_H

#include "mapa.h"
#include "player.h" 
#include <stdbool.h>

void IA_IniciaCena(SceneNode* cena, Player* jogador);

void IA_Update(float deltaTime);

int IA_GetTotalInimigos();

bool IA_EstaAtiva();

#endif 