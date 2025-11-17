#include "player.h"
#include "bullet.h"
#include "mapa.h"
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h> 
#include "globals.h"  
#include "item.h"
#include "caixa.h"
#include "mode_select.h" 
#include "algoritmo_inimigos.h" 

#define PLAYER_ANIM_SPEED_PARADO 50
#define PLAYER_ANIM_SPEED_ANDANDO 15

/**
 * @brief Inicializa a struct Player com os atributos (velocidade, vida, texturas)
 * específicos do personagem selecionado.
 */
void InitPlayer(Player *player, CharacterType charType, int startX, int startY) {
    player->position = (Vector2){ (float)startX, (float)startY };
    player->scale = 4.0f;
    player->direction = 1;
    player->isMoving = false;
    player->isAttacking = false;
    player->currentFrame = 0;
    player->framesCounter = 0;
    player->framesSpeed = PLAYER_ANIM_SPEED_PARADO;
    player->isAlive = true; 
    player->charType = charType; 

    player->isReloading = false;
    player->reloadTimer = 0.0f;
    player->collisionDamageTimer = 0.0f; 

    player->extraLives = 1; // Cada jogador começa com 1 vida extra

    player->attackSound = (Sound){ 0 };
    
    char path[256];

    // Carrega os assets específicos de cada personagem
    switch (charType) {
        case CHAR_JOHNNY:
            player->speed = 4.0f;
            player->maxHealth = 150;

            player->idleFrameCount = 2;
            player->walkFrameCount = 3;
            player->attackFrameCount = 1;
            player->attackAnimSpeed = 20;
            
            player->idleTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->idleFrameCount);
            player->walkTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->walkFrameCount);
            player->attackTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->attackFrameCount);
            
            for (int i = 0; i < player->idleFrameCount; i++) {
                sprintf(path, "assets/Sprites/JohnyBravo/parado/JohnyBravo_parado%d.png", i + 1); 
                player->idleTextures[i] = LoadTexture(path);
            }
            for (int i = 0; i < player->walkFrameCount; i++) {
                sprintf(path, "assets/Sprites/JohnyBravo/movimentacao/JohnyBravo_walking%d.png", i + 1); 
                player->walkTextures[i] = LoadTexture(path);
            }
            for (int i = 0; i < player->attackFrameCount; i++) {
                sprintf(path, "assets/Sprites/JohnyBravo/ataque/JohnyBravo_ataque%d.png", i + 1); 
                player->attackTextures[i] = LoadTexture(path);
            }
            break;
        
        case CHAR_FINN:
            player->speed = 6.0f;  
            player->maxHealth = 100; // Vida baixa, alta velocidade

            player->idleFrameCount = 2; 
            player->walkFrameCount = 4;
            player->attackFrameCount = 2; 
            player->attackAnimSpeed = 10;
            
            player->idleTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->idleFrameCount);
            player->walkTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->walkFrameCount);
            player->attackTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->attackFrameCount);

            player->attackSound = LoadSound("assets/audios/swordSound.wav");
            SetSoundVolume(player->attackSound, 0.6f);

            player->idleTextures[0] = LoadTexture("assets/Sprites/Finn/Finnparado/finnparado1.png");
            player->idleTextures[1] = LoadTexture("assets/Sprites/Finn/Finnparado/finn3.png");
            
            player->walkTextures[0] = LoadTexture("assets/Sprites/Finn/Finnmovimentacao/finnandando1.png");
            player->walkTextures[1] = LoadTexture("assets/Sprites/Finn/Finnmovimentacao/finnandando2.png");
            player->walkTextures[2] = LoadTexture("assets/Sprites/Finn/Finnmovimentacao/finnandando3.png");
            player->walkTextures[3] = LoadTexture("assets/Sprites/Finn/Finnmovimentacao/finnandando4.png");

            player->attackTextures[0] = LoadTexture("assets/Sprites/Finn/Finnataque/finnataque1.png");
            player->attackTextures[1] = LoadTexture("assets/Sprites/Finn/Finnataque/finnataque2.png");
            break;

        case CHAR_GARNET: 
            player->speed = 4.0f;  
            player->maxHealth = 150; 

            player->idleFrameCount = 2;
            player->walkFrameCount = 4; 
            player->attackFrameCount = 2;
            player->attackAnimSpeed = 15; 
            
            player->idleTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->idleFrameCount);
            player->walkTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->walkFrameCount);
            player->attackTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->attackFrameCount);

            player->attackSound = LoadSound("assets/audios/punchSound.wav");
            SetSoundVolume(player->attackSound, 0.9f);
            
            player->idleTextures[0] = LoadTexture("assets/Sprites/Garnet/Garnetparada/Garnet_parada1.png");
            player->idleTextures[1] = LoadTexture("assets/Sprites/Garnet/Garnetparada/Garnet_parada2.png");

            player->walkTextures[0] = LoadTexture("assets/Sprites/Garnet/Garnetmovimentacao/Garnet_andando1.png");
            player->walkTextures[1] = LoadTexture("assets/Sprites/Garnet/Garnetmovimentacao/Garnet_andando2.png");
            player->walkTextures[2] = LoadTexture("assets/Sprites/Garnet/Garnetmovimentacao/Garnet_andando3.png");
            player->walkTextures[3] = LoadTexture("assets/Sprites/Garnet/Garnetmovimentacao/Garnet_andando4.png");

            player->attackTextures[0] = LoadTexture("assets/Sprites/Garnet/Garnetataque/Garnet_ataque1.png");
            player->attackTextures[1] = LoadTexture("assets/Sprites/Garnet/Garnetataque/Garnet_ataque2.png");
            break;
            
        case CHAR_MORDECAI:
            player->speed = 5.5f;
            player->maxHealth = 130;
            
            player->idleFrameCount = 2; 
            player->walkFrameCount = 6; 
            player->attackFrameCount = 2; 
            player->attackAnimSpeed = 10;
            
            player->idleTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->idleFrameCount);
            player->walkTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->walkFrameCount);
            player->attackTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->attackFrameCount);

            player->attackSound = LoadSound("assets/audios/punchSound.wav");
            SetSoundVolume(player->attackSound, 0.9f);
            
            player->idleTextures[0] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladoparado/mordecaidescoladoparado.png");
            player->idleTextures[1] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladoparado/mordecai51.png");

            player->walkTextures[0] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladomovimentacao/mordecaidescoladocorrendo1.png");
            player->walkTextures[1] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladomovimentacao/mordecaidescoladocorrendo2.png");
            player->walkTextures[2] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladomovimentacao/mordecaidescoladocorrendo3.png");
            player->walkTextures[3] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladomovimentacao/mordecaidescoladocorrendo4.png");
            player->walkTextures[4] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladomovimentacao/mordecaidescoladocorrendo5.png");
            player->walkTextures[5] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladomovimentacao/mordecaidescoladocorrendo6.png");

            player->attackTextures[0] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladoataque/mordecaidescoladoataque1.png");
            player->attackTextures[1] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladoataque/mordecaidescoladoataque2.png");
            break;
    }

    player->health = player->maxHealth;
}

/**
 * @brief Atualiza a lógica do jogador a cada frame.
 * Processa inputs (movimento, ataque, recarga), atualiza animações,
 * checa colisão com balas de inimigos e mantém o jogador dentro dos limites da cena.
 * @return Retorna 'true' se o jogador alcançou o fim da tela (para mudar de cena), 'false' caso contrário.
 */
bool UpdatePlayer(Player *player, int screenWidth, int screenHeight, SceneNode* currentScene, bool isPlayer2) {

    if (!player->isAlive) return false; 

    // Timer de invencibilidade após tomar dano
    if (player->collisionDamageTimer > 0) {
        player->collisionDamageTimer -= GetFrameTime();
    }

    int playerOwner = isPlayer2 ? 2 : 1;

    // Lógica de Recarga (Johnny)
    if (player->isReloading) {
        player->reloadTimer -= GetFrameTime();
        if (player->reloadTimer <= 0.0f) {
            ReloadAmmo(playerOwner); 
            player->isReloading = false;
        }
    }

    // Lógica de Ataque (Input)
    bool attackPressed = (!isPlayer2 && IsKeyPressed(KEY_SPACE)) || (isPlayer2 && IsKeyPressed(KEY_L));
    if (attackPressed && !player->isAttacking && !player->isReloading) {
        player->isAttacking = true;
        player->currentFrame = 0;
        player->framesCounter = 0;
        player->framesSpeed = player->attackAnimSpeed;
        
        // Define a posição de onde a bala/soco se origina
        Texture2D attackTexture = player->attackTextures[0];
        float playerHeight = (float)attackTexture.height * player->scale;
        float playerWidth = (float)attackTexture.width * player->scale;
        Vector2 startPos;
        float offsetY = playerHeight * 0.38f; 
        float offsetX_Right = playerWidth * 1.0f;
        float offsetX_Left = playerWidth * 0.2f; 
        startPos.y = player->position.y + offsetY;
        startPos.y = player->position.y + offsetY;
        if (player->direction == 1) { 
            startPos.x = player->position.x + offsetX_Right;
        } else { 
            startPos.x = player->position.x + offsetX_Left;
        }

        // Spawna bala (Johnny) ou toca som (Melee)
        if (player->charType == CHAR_JOHNNY) { 
            SpawnBullet(startPos, player->direction, playerOwner); 
        } 
        else {
            if (player->attackSound.frameCount > 0) { 
                PlaySound(player->attackSound);
            }
        }
    }

    // Lógica de Recarregar (Input)
    bool reloadPressed = (!isPlayer2 && IsKeyPressed(KEY_R)) || (isPlayer2 && IsKeyPressed(KEY_K));
    if (reloadPressed && player->charType == CHAR_JOHNNY) {
        if (!player->isReloading && GetCurrentAmmo(playerOwner) < 10) { 
            player->isReloading = true;
            player->reloadTimer = 1.5f; 
        }
    }

    // Lógica de Movimento (Input)
    bool wasMoving = player->isMoving;
    player->isMoving = false;

    if (!player->isAttacking) { // Não pode se mover enquanto ataca
        if (!isPlayer2) {
            if (IsKeyDown(KEY_W)) { player->position.y -= player->speed; player->isMoving = true; }
            if (IsKeyDown(KEY_S)) { player->position.y += player->speed; player->isMoving = true; }
            if (IsKeyDown(KEY_A)) { player->position.x -= player->speed; player->isMoving = true; player->direction = -1; }
            if (IsKeyDown(KEY_D)) { player->position.x += player->speed; player->isMoving = true; player->direction = 1; }
        } else {
            // Controles P2
            if (IsKeyDown(KEY_UP)) { player->position.y -= player->speed; player->isMoving = true; }
            if (IsKeyDown(KEY_DOWN)) { player->position.y += player->speed; player->isMoving = true; }
            if (IsKeyDown(KEY_LEFT)) { player->position.x -= player->speed; player->isMoving = true; player->direction = -1; }
            if (IsKeyDown(KEY_RIGHT)) { player->position.x += player->speed; player->isMoving = true; player->direction = 1; }
        }
    }
    
    // Lógica de Animação (Avança os frames)
    int currentAnimFrameCount = 0;

    if (player->isAttacking) {
        currentAnimFrameCount = player->attackFrameCount;
        
        player->framesCounter++;
        if (player->framesCounter >= player->framesSpeed) {
            player->framesCounter = 0;
            player->currentFrame++;
            if (player->currentFrame >= currentAnimFrameCount) {
                player->isAttacking = false;
                player->currentFrame = 0;
                player->framesSpeed = PLAYER_ANIM_SPEED_PARADO;
            }
        }
    }
    else { // Animação de Parado ou Andando
        if (player->isMoving) {
            currentAnimFrameCount = player->walkFrameCount;
        } else {
            currentAnimFrameCount = player->idleFrameCount;
        }

        if (player->isMoving != wasMoving) {
            player->currentFrame = 0;
            player->framesCounter = 0;
            player->framesSpeed = player->isMoving ? PLAYER_ANIM_SPEED_ANDANDO : PLAYER_ANIM_SPEED_PARADO;
        }

        player->framesCounter++;
        if (player->framesCounter >= player->framesSpeed) {
            player->framesCounter = 0;
            player->currentFrame++;
            if (player->currentFrame >= currentAnimFrameCount) {
                player->currentFrame = 0;
            }
        }
    }

    // Checa colisão com balas inimigas
    int damageTaken = 0;
    Rectangle playerRect = GetPlayerRect(player);
    
    if (CheckEnemyBulletCollision(playerRect, &damageTaken)) { 
        player->health -= damageTaken;
    }

    // Lógica de Limites do Mapa
    Texture2D currentTexture = GetPlayerCurrentTexture(player);
    float playerWidth = (float)currentTexture.width * player->scale;
    float playerHeight = (float)currentTexture.height * player->scale;

    float limiteDireito = (float)screenWidth; 
    if (currentScene != NULL && currentScene->id == 5) {
        limiteDireito = 1300.0f; // Limite da arena do boss
    }

    // Lógica de Transição de Cena (Borda Direita)
    if (player->position.x + playerWidth > limiteDireito) {
        if (currentScene != NULL && currentScene->id == 5) {
            player->position.x = limiteDireito - playerWidth;
        } 
        else {
            SceneNode* current = GetCurrentScene();
            if (current->next != NULL) {
                
                if (AreAllEnemiesDefeated() && !ControleSpawn_EstaAtivo()) { 
                    return true; // Sinaliza para 'jogo.c' que pode mudar de cena
                } else {
                    player->position.x = screenWidth - playerWidth; // Bate na parede
                }
            } else {
                player->position.x = screenWidth - playerWidth; // Borda final
            }
        }
    }
    else if (player->position.x < 0) { // Borda Esquerda
            player->position.x = 0;
    }

    // Limites da "Rua" (Cima e Baixo)
    float limiteInferior = (float)screenHeight; 

    if (currentScene != NULL && currentScene->id == 5) {
        limiteInferior = 850.0f; // Chão da arena do boss
    }
    if (player->position.y < RUA_LIMITE_SUPERIOR) {
        player->position.y = RUA_LIMITE_SUPERIOR;
    }
    else if (player->position.y + playerHeight > limiteInferior) {
        player->position.y = limiteInferior - playerHeight;
    }

    // Cheat (Remover?)
    if (IsKeyPressed(KEY_SLASH)) {
        player->health += 20;
        if (player->health < 0) player->health = 0;
    }

    return false; // Não quer mudar de cena
}

/**
 * @brief Desenha o sprite do jogador na tela, espelhado corretamente.
 */
void DrawPlayer(const Player *player) {
    if (!player->isAlive) return;

    Texture2D textureToDraw;
    if (player->isAttacking) {
        textureToDraw = player->attackTextures[player->currentFrame];
    } else if (player->isMoving) {
        textureToDraw = player->walkTextures[player->currentFrame];
    } else {
        textureToDraw = player->idleTextures[player->currentFrame];
    }
    
    Rectangle sourceRec = { 0.0f, 0.0f, (float)textureToDraw.width, (float)textureToDraw.height };
    if (player->direction == -1) {
        sourceRec.width = -sourceRec.width; // Espelha a textura
    }
    Rectangle destRec = {
        player->position.x, player->position.y,
        (float)textureToDraw.width * player->scale,
        (float)textureToDraw.height * player->scale
    };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(textureToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
}
  
/**
 * @brief Desenha a barra de vida do jogador (P1 ou P2) e
 * a contagem de vidas extras (individual) logo abaixo.
 */
void DrawPlayerHealthBar(const Player *player, bool isPlayer2) {
    int barWidth = 200;
    int barHeight = 20;
    int barX;
    int barY = 20; // Posição Y da barra de vida

    if (!isPlayer2) {
        barX = 20; // Canto superior esquerdo
    } else {
        barX = GetScreenWidth() - barWidth - 20; // Canto superior direito
    }

    float healthPercentage = (float)player->health / (float)player->maxHealth;
    int currentHealthWidth = (int)(barWidth * healthPercentage);
    
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, currentHealthWidth, barHeight, GREEN);
    DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);


    // Lógica de Posição do Texto de Vidas
    int livesTextY;
    
    if (player->charType == CHAR_JOHNNY) {
        // Se for o Johnny, coloca "LIVES" abaixo da munição (que é desenhada em y=50)
        livesTextY = 50 + 20 + 5; 
    } else {
        // Para outros, coloca "LIVES" abaixo da barra de vida
        livesTextY = barY + barHeight + 5; 
    }

    const char* livesText = TextFormat("VIDAS EXTRA: %d", player->extraLives); 
    DrawText(livesText, barX, livesTextY, 20, RAYWHITE);
}

/**
 * @brief Retorna a textura de animação correta para o estado atual do jogador.
 */
Texture2D GetPlayerCurrentTexture(const Player *player) {
    if (player->isAttacking) {
        return player->attackTextures[player->currentFrame];
    } else if (player->isMoving) {
        return player->walkTextures[player->currentFrame];
    } else {
        return player->idleTextures[player->currentFrame];
    }
}

/**
 * @brief Retorna o retângulo de colisão principal do jogador (hitbox).
 */
Rectangle GetPlayerRect(const Player *player) {
    Texture2D baseTexture = player->idleTextures[0]; // Usa a textura base para tamanho consistente

    Rectangle rect = {
        player->position.x, player->position.y,
        (float)baseTexture.width * player->scale,
        (float)baseTexture.height * player->scale
    };
    return rect;
}

/**
 * @brief Retorna o retângulo de colisão do ATAQUE MELEE (soco/espada).
 * Retorna um retângulo vazio se for o Johnny Bravo ou se não estiver atacando.
 */
Rectangle GetPlayerMeleeRect(Player *player) {
    if (!player->isAlive) {
        return (Rectangle){ 0, 0, 0, 0 };
    }

    if (!player->isAttacking || player->charType == CHAR_JOHNNY) {
        return (Rectangle){ 0, 0, 0, 0 };
    }

    #define MELEE_RANGE 80.0f
    #define MELEE_HEIGHT 100.0f

    Rectangle playerRect = GetPlayerRect(player);
    Rectangle meleeRect;

    // Define a hitbox na frente do jogador
    meleeRect.y = playerRect.y + (playerRect.height / 2) - (MELEE_HEIGHT / 2);
    meleeRect.width = MELEE_RANGE;
    meleeRect.height = MELEE_HEIGHT;

    if (player->direction == 1) { // Direita
        meleeRect.x = playerRect.x + playerRect.width;
    } else { // Esquerda
        meleeRect.x = playerRect.x - MELEE_RANGE;
    }

    return meleeRect;
}

/**
 * @brief Libera da memória todas as texturas e sons carregados para este jogador.
 */
void UnloadPlayer(Player *player) {
    for (int i = 0; i < player->idleFrameCount; i++) {
        UnloadTexture(player->idleTextures[i]);
    }
    for (int i = 0; i < player->walkFrameCount; i++) {
        UnloadTexture(player->walkTextures[i]);
    }
    for (int i = 0; i < player->attackFrameCount; i++) {
        UnloadTexture(player->attackTextures[i]);
    }

    free(player->idleTextures);
    free(player->walkTextures);
    free(player->attackTextures);

    if (player->attackSound.frameCount > 0) {
        UnloadSound(player->attackSound);
    }
}

/**
 * @brief Verifica se o jogador está no estado de recarga (apenas Johnny).
 */
bool Player_IsReloading(const Player *player) {
    return player->isReloading;
}

/**
 * @brief Aplica dano ao jogador e ativa um curto período de invencibilidade (i-frames).
 */
void Player_RecebeDano(Player *player, int dano) {
    if (player == NULL) return; 
    if (player->collisionDamageTimer > 0) {
        return; // Está invencível
    }

    player->health -= dano;
    if (player->health < 0) {
        player->health = 0;
    }
    player->collisionDamageTimer = 0.5f; // Meio segundo de invencibilidade
}