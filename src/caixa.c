#include "caixa.h"
#include "item.h"     
#include "bullet.h"   
#include "globals.h"  
#include "player.h"   
#include <stdlib.h>   
#include <stdio.h>    

#define MAX_CAIXAS 2        // REQUISIÇÃO: Alterado de 1 para 2
#define CAIXA_HEALTH 3      
#define CAIXA_SCALE 3.0f

typedef struct Caixa {
    bool active;
    Vector2 position;
    int health;
    float hitStunTimer; 
} Caixa;

static Caixa caixaPool[MAX_CAIXAS];
static Texture2D caixaTexture;

static Rectangle GetCaixaRect(const Caixa* caixa) {
    if (!caixa->active) return (Rectangle){0,0,0,0};
    return (Rectangle){
        caixa->position.x, caixa->position.y,
        (float)caixaTexture.width * CAIXA_SCALE,
        (float)caixaTexture.height * CAIXA_SCALE
    };
}

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

void Caixa_Init(void) {
    caixaTexture = LoadTexture("assets/Sprites/itens/caixa.png");
    for (int i = 0; i < MAX_CAIXAS; i++) {
        caixaPool[i].active = false;
    }
}

void Caixa_Unload(void) {
    UnloadTexture(caixaTexture);
}

void Caixa_Update(Player *player) {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        Caixa *caixa = &caixaPool[i];
        if (!caixa->active) continue;

        if (caixa->hitStunTimer > 0) {
            caixa->hitStunTimer -= GetFrameTime();
        }

        Rectangle caixaRect = GetCaixaRect(caixa);
        bool hit = false;
        int damageTaken = 0; 

        if (selectedCharacter == CHAR_JOHNNY) {
            if (CheckBulletCollision(caixaRect, &damageTaken)) {
                hit = true;
            }
        } else {
            Rectangle meleeRect = GetPlayerMeleeRect(player);
            if (player->isAttacking && meleeRect.width > 0 && caixa->hitStunTimer <= 0 && CheckCollisionRecs(meleeRect, caixaRect)) {
                hit = true;
                caixa->hitStunTimer = 0.5f; 
            }
        }

        if (hit) {
            caixa->health--;
            if (caixa->health <= 0) {
                caixa->active = false;

                ItemType itemDrop = (ItemType)(rand() % 3); 
                Item_Spawn(caixa->position, itemDrop);
            }
        }
    }
}

void Caixa_Draw(void) {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        if (caixaPool[i].active) {
            DrawTextureEx(caixaTexture, caixaPool[i].position, 0.0f, CAIXA_SCALE, WHITE);
        }
    }
}

void Caixa_DespawnAll(void) {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        caixaPool[i].active = false;
    }
}