#include "caixa.h"
#include "item.h"     
#include "bullet.h"   
#include "globals.h"  
#include "player.h"   
#include <stdlib.h>   
#include <stdio.h>    

#define MAX_CAIXAS 2        
#define CAIXA_HEALTH 3      
#define CAIXA_SCALE 3.0f

typedef struct Caixa {
    bool active;
    Vector2 position;
    int health;
    float hitStunTimer; // Temporizador para evitar hits múltiplos
} Caixa;

static Caixa caixaPool[MAX_CAIXAS];
static Texture2D caixaTexture;

// Retorna o retângulo de colisão para uma caixa específica.
 
static Rectangle GetCaixaRect(const Caixa* caixa) {
    if (!caixa->active) return (Rectangle){0,0,0,0};
    return (Rectangle){
        caixa->position.x, caixa->position.y,
        (float)caixaTexture.width * CAIXA_SCALE,
        (float)caixaTexture.height * CAIXA_SCALE
    };
}

// Ativa uma caixa do "pool" em uma posição específica do mapa.
 
void Caixa_Spawn_At(Vector2 position) {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        if (!caixaPool[i].active) {
            Caixa *caixa = &caixaPool[i];
            
            caixa->position = position;
            caixa->active = true;
            caixa->health = CAIXA_HEALTH;
            caixa->hitStunTimer = 0.0f;
            
            return; 
        }
    }
}

// Carrega a textura da caixa e inicializa o "pool" de caixas.
 
void Caixa_Init(void) {
    caixaTexture = LoadTexture("assets/Sprites/itens/caixa.png");
    for (int i = 0; i < MAX_CAIXAS; i++) {
        caixaPool[i].active = false;
    }
}

// Libera a textura da caixa da memória.
 
void Caixa_Unload(void) {
    UnloadTexture(caixaTexture);
}

/*
  Atualiza a lógica da caixa a cada frame.
  Verifica colisões com balas ou ataques melee do P1 e P2.
  Se a vida da caixa chegar a zero, ela "dropa" um item aleatório.
 */
void Caixa_Update(Player *player1, Player *player2, bool isPlayer2Active) {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        Caixa *caixa = &caixaPool[i];
        if (!caixa->active) continue;

        if (caixa->hitStunTimer > 0) {
            caixa->hitStunTimer -= GetFrameTime();
        }

        Rectangle caixaRect = GetCaixaRect(caixa);
        bool hit = false;
        int damageTaken = 0; 

        // Checa colisão com balas (de qualquer jogador)
        if (CheckBulletCollision(caixaRect, &damageTaken)) {
            hit = true;
        }

        // Checa colisão com ataque melee P1
        if (!hit && player1->isAlive && player1->charType != CHAR_JOHNNY && player1->isAttacking && caixa->hitStunTimer <= 0) {
            Rectangle meleeRectP1 = GetPlayerMeleeRect(player1);
            if (meleeRectP1.width > 0 && CheckCollisionRecs(meleeRectP1, caixaRect)) {
                hit = true;
            }
        }
        
        // Checa colisão com ataque melee P2
        if (!hit && isPlayer2Active && player2->isAlive && player2->charType != CHAR_JOHNNY && player2->isAttacking && caixa->hitStunTimer <= 0) {
            Rectangle meleeRectP2 = GetPlayerMeleeRect(player2);
            if (meleeRectP2.width > 0 && CheckCollisionRecs(meleeRectP2, caixaRect)) {
                hit = true;
            }
        }

        if (hit) {
            caixa->hitStunTimer = 0.5f; // Tempo de "hit stun" para evitar hits múltiplos
            caixa->health--;
            
            if (caixa->health <= 0) {
                caixa->active = false;

                // Spawna um item aleatório (Maçã, Dinheiro ou Vida Extra)
                ItemType itemDrop = (ItemType)(rand() % 3); 
                Item_Spawn(caixa->position, itemDrop);
            }
        }
    }
}

// Desenha todas as caixas ativas na tela.
 
void Caixa_Draw(void) {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        if (caixaPool[i].active) {
            DrawTextureEx(caixaTexture, caixaPool[i].position, 0.0f, CAIXA_SCALE, WHITE);
        }
    }
}

// Desativa todas as caixas (usado na troca de cena).
 
void Caixa_DespawnAll(void) {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        caixaPool[i].active = false;
    }
}