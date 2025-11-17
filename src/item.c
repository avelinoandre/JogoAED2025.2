#include "item.h"
#include "raylib.h"
#include "globals.h"
#include <stdlib.h> 
#include "score.h" // ADICIONADO: Necessário para Score_AddPoints

#define MAX_ITENS 10
#define ITEM_LIFESPAN 8.0f // Itens desaparecem depois de 8 segundos
#define ITEM_BLINK_TIME 2.0f // Começa a piscar nos últimos 2 segundos

static Item itemPool[MAX_ITENS];
static Texture2D macaTexture;
static Texture2D dinheiroTexture;
static Texture2D vidaExtraTexture;

static Sound itemSound;
static Sound vidaExtraSound; // Som separado para vida extra

// Contador para o limite de vidas extras por jogo
static int extraLivesSpawnedThisGame = 0;

/**
 * @brief Carrega as texturas e sons dos itens.
 * Reseta o contador de vidas extras spawnadas (para um novo jogo).
 */
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

    // Reseta o contador de vidas extras para o novo jogo
    extraLivesSpawnedThisGame = 0;
}

/**
 * @brief Libera as texturas e sons dos itens da memória.
 */
void Item_Unload(void) {
    UnloadTexture(macaTexture);
    UnloadTexture(dinheiroTexture);
    UnloadTexture(vidaExtraTexture);
    UnloadSound(itemSound);
    UnloadSound(vidaExtraSound);
}

/**
 * @brief Ativa um item no "pool" em uma posição.
 * Verifica o limite de vidas extras (1 para 1P, 2 para 2P).
 * Se o limite foi atingido, substitui a vida extra por Maçã ou Dinheiro.
 */
void Item_Spawn(Vector2 position, ItemType type) {
    
    // Lógica de Limite de Vida Extra
    if (type == ITEM_VIDA_EXTRA) {
        bool is2P = (selectedGameMode == GAME_MODE_2P);
        int maxLivesToSpawn = is2P ? 2 : 1; // 1 para 1P, 2 para 2P

        if (extraLivesSpawnedThisGame >= maxLivesToSpawn) {
            // Limite atingido, substitui o item
            type = (rand() % 2 == 0) ? ITEM_MACA : ITEM_DINHEIRO;
        } else {
            // Ainda pode spawnar, incrementa o contador
            extraLivesSpawnedThisGame++;
        }
    }

    // Encontra um slot vazio no pool para o item
    for (int i = 0; i < MAX_ITENS; i++) {
        if (!itemPool[i].active) {
            itemPool[i].active = true;
            itemPool[i].position = position;
            itemPool[i].type = type; // 'type' pode ter sido modificado
            itemPool[i].timer = ITEM_LIFESPAN;
            
            Texture2D tex;
            float itemScale; 

            // Define a textura e a escala com base no tipo
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
            
            // Define o retângulo de colisão com base na escala
            itemPool[i].rect = (Rectangle){
                position.x, position.y,
                (float)tex.width * itemScale,
                (float)tex.height * itemScale
            };
            return; 
        }
    }
}

/**
 * @brief Atualiza os itens. Verifica a colisão com P1 e P2.
 * Se um item de Vida Extra for pego e o outro jogador estiver morto (em 2P),
 * o item revive o jogador morto com 50% da vida.
 * Se não, o coletor ganha os efeitos (cura, pontos ou vida extra).
 */
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
            
            // Contagem regressiva para o item desaparecer
            itemPool[i].timer -= GetFrameTime();
            if (itemPool[i].timer <= 0.0f) {
                itemPool[i].active = false;
                continue;
            }

            bool itemColetado = false;
            Player* coletor = NULL;

            // Checa colisão com P1
            if (player1->isAlive && CheckCollisionRecs(player1Rect, itemPool[i].rect)) {
                itemColetado = true;
                coletor = player1;
            } 
            // Checa colisão com P2 (apenas se P1 não pegou)
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
                        Score_AddPoints(1000); // Adiciona pontos
                        PlaySound(itemSound);
                        break;
                    
                    case ITEM_VIDA_EXTRA:
                        { // Bloco 'case' necessário para declarar variáveis
                            bool reviveuAlguem = false;
                            
                            if (isPlayer2Active) {
                                if (coletor == player1 && !player2->isAlive) {
                                    // P1 pegou, P2 está morto -> Revive P2
                                    player2->isAlive = true;
                                    player2->health = player2->maxHealth / 1.4; // Revive com ~70% vida
                                    reviveuAlguem = true;
                                } else if (coletor == player2 && !player1->isAlive) {
                                    // P2 pegou, P1 está morto -> Revive P1
                                    player1->isAlive = true;
                                    player1->health = player1->maxHealth / 1.4; // Revive com ~70% vida
                                    reviveuAlguem = true;
                                }
                            }

                            if (!reviveuAlguem) {
                                // Se ninguém foi revivido, adiciona vida extra ao coletor
                                coletor->extraLives++; 
                            }
                            
                            PlaySound(vidaExtraSound);
                        }
                        break;
                }
                itemPool[i].active = false; // Desativa o item após ser coletado
            }
        }
    }
}

/**
 * @brief Desenha todos os itens ativos na tela, usando sua escala individual.
 * Aplica um efeito de "piscar" quando o item está prestes a sumir.
 */
void Item_Draw(void) {
    for (int i = 0; i < MAX_ITENS; i++) {
        if (itemPool[i].active) {
            
            // Efeito de piscar
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

/**
 * @brief Desativa todos os itens (usado na troca de cena).
 */
void Item_DespawnAll(void) {
    for (int i = 0; i < MAX_ITENS; i++) {
        itemPool[i].active = false;
    }
}