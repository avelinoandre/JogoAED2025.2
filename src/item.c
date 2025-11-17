#include "item.h"
#include "raylib.h"
#include "globals.h"
#include <stdlib.h> 
#include "score.h"

#define MAX_ITENS 10
#define ITEM_LIFESPAN 8.0f 
#define ITEM_BLINK_TIME 2.0f 

static Item itemPool[MAX_ITENS];
static Texture2D macaTexture;
static Texture2D dinheiroTexture;
static Texture2D vidaExtraTexture;

static Sound itemSound;
static Sound vidaExtraSound; 

static int extraLivesSpawnedThisGame = 0;

void Item_Init(void) {
    for (int i = 0; i < MAX_ITENS; i++) {
        itemPool[i].active = false;
    }
    macaTexture = LoadTexture("assets/Sprites/itens/maca.png");
    dinheiroTexture = LoadTexture("assets/Sprites/itens/dinheiro.png");
    vidaExtraTexture = LoadTexture("assets/Sprites/itens/vidaExtra.png");
    
    itemSound = LoadSound("assets/audios/itemSound.wav");
    SetSoundVolume(itemSound, 0.6f);
    vidaExtraSound = LoadSound("assets/audios/powerUp.wav"); 
    SetSoundVolume(vidaExtraSound, 0.6f);

    extraLivesSpawnedThisGame = 0;
}

void Item_Unload(void) {
    UnloadTexture(macaTexture);
    UnloadTexture(dinheiroTexture);
    UnloadTexture(vidaExtraTexture);
    UnloadSound(itemSound);
    UnloadSound(vidaExtraSound);
}

void Item_Spawn(Vector2 position, ItemType type) {
    
    if (type == ITEM_VIDA_EXTRA) {
        bool is2P = (selectedGameMode == GAME_MODE_2P);
        int maxLivesToSpawn = is2P ? 2 : 1; 

        if (extraLivesSpawnedThisGame >= maxLivesToSpawn) {
            type = (rand() % 2 == 0) ? ITEM_MACA : ITEM_DINHEIRO;
        } else {
            extraLivesSpawnedThisGame++;
        }
    }

    for (int i = 0; i < MAX_ITENS; i++) {
        if (!itemPool[i].active) {
            itemPool[i].active = true;
            itemPool[i].position = position;
            itemPool[i].type = type; 
            itemPool[i].timer = ITEM_LIFESPAN;
            
            Texture2D tex;
            float itemScale; 

            switch(type) {
                case ITEM_MACA: 
                    tex = macaTexture; 
                    itemScale = 1.2f; 
                    break;
                case ITEM_DINHEIRO: 
                    tex = dinheiroTexture; 
                    itemScale = 3.0f;
                    break;
                case ITEM_VIDA_EXTRA: 
                    tex = vidaExtraTexture; 
                    itemScale = 3.0f;
                    break;
            }
            
            itemPool[i].scale = itemScale;
            
            itemPool[i].rect = (Rectangle){
                position.x, position.y,
                (float)tex.width * itemScale,
                (float)tex.height * itemScale
            };
            return; 
        }
    }
}

void Item_Update(Player *player1, Player *player2, bool isPlayer2Active) {
    
    Rectangle player1Rect = {0};
    if (player1->isAlive) {
        player1Rect = GetPlayerRect(player1);
    }

    Rectangle player2Rect = {0};
    if (isPlayer2Active && player2->isAlive) {
        player2Rect = GetPlayerRect(player2);
    }

    for (int i = 0; i < MAX_ITENS; i++) {
        if (itemPool[i].active) {
            
            itemPool[i].timer -= GetFrameTime();
            if (itemPool[i].timer <= 0.0f) {
                itemPool[i].active = false;
                continue;
            }

            bool itemColetado = false;
            Player* coletor = NULL;

            if (player1->isAlive && CheckCollisionRecs(player1Rect, itemPool[i].rect)) {
                itemColetado = true;
                coletor = player1;
            } 
            else if (isPlayer2Active && player2->isAlive && CheckCollisionRecs(player2Rect, itemPool[i].rect)) {
                itemColetado = true;
                coletor = player2;
            }

            if (itemColetado) {
                switch (itemPool[i].type) {
                    case ITEM_MACA:
                        coletor->health += 50; 
                        if (coletor->health > coletor->maxHealth) {
                            coletor->health = coletor->maxHealth;
                        }
                        PlaySound(itemSound);
                        break;
                    
                    case ITEM_DINHEIRO:
                        Score_AddPoints(500); 
                        PlaySound(itemSound);
                        break;
                    
                    case ITEM_VIDA_EXTRA:
                        { 
                            bool reviveuAlguem = false;
                            
                            if (isPlayer2Active) {
                                if (coletor == player1 && !player2->isAlive) {
                                    player2->isAlive = true;
                                    player2->health = player2->maxHealth / 1.4; 
                                    reviveuAlguem = true;
                                } else if (coletor == player2 && !player1->isAlive) {

                                    player1->isAlive = true;
                                    player1->health = player1->maxHealth / 1.4; 
                                    reviveuAlguem = true;
                                }
                            }

                            if (!reviveuAlguem) {
                                coletor->extraLives++; 
                            }
                            
                            PlaySound(vidaExtraSound);
                        }
                        break;
                }
                itemPool[i].active = false; 
            }
        }
    }
}

void Item_Draw(void) {
    for (int i = 0; i < MAX_ITENS; i++) {
        if (itemPool[i].active) {
            
            bool piscar = (itemPool[i].timer <= ITEM_BLINK_TIME) && ((int)(itemPool[i].timer * 10) % 2 == 0);
            if (piscar) continue; 

            Texture2D tex;
            switch (itemPool[i].type) {
                case ITEM_MACA: tex = macaTexture; break;
                case ITEM_DINHEIRO: tex = dinheiroTexture; break;
                case ITEM_VIDA_EXTRA: tex = vidaExtraTexture; break;
            }
            
            DrawTextureEx(tex, itemPool[i].position, 0.0f, itemPool[i].scale, WHITE);
        }
    }
}

void Item_DespawnAll(void) {
    for (int i = 0; i < MAX_ITENS; i++) {
        itemPool[i].active = false;
    }
}