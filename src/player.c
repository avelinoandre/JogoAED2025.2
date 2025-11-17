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
    player->collisionDamageTimer = 0.0f; // Adicionado para inicializar

    player->attackSound = (Sound){ 0 };
    
    char path[256];

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
            player->maxHealth = 120; 

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

        case CHAR_SAMURAI:

            player->speed = 4.5f;  
            player->maxHealth = 150;

            player->idleFrameCount = 2;
            player->walkFrameCount = 4; 
            player->attackFrameCount = 2;
            player->attackAnimSpeed = 20; 
            
            player->idleTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->idleFrameCount);
            player->walkTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->walkFrameCount);
            player->attackTextures = (Texture2D*)malloc(sizeof(Texture2D) * player->attackFrameCount);

            player->attackSound = LoadSound("assets/audios/swordSound.wav");
            SetSoundVolume(player->attackSound, 0.6f);
            
            player->idleTextures[0] = LoadTexture("assets/Sprites/Samurai/Samuraiparado/Samuraiparado1.png");
            player->idleTextures[1] = LoadTexture("assets/Sprites/Samurai/Samuraiparado/Samurai3.png");

            player->walkTextures[0] = LoadTexture("assets/Sprites/Samurai/Samuraimovimentacao/Samuraiandando1.png");
            player->walkTextures[1] = LoadTexture("assets/Sprites/Samurai/Samuraimovimentacao/Samuraiandando2.png");
            player->walkTextures[2] = LoadTexture("assets/Sprites/Samurai/Samuraimovimentacao/Samuraiandando3.png");
            player->walkTextures[3] = LoadTexture("assets/Sprites/Samurai/Samuraimovimentacao/Samuraiandando4.png");

            player->attackTextures[0] = LoadTexture("assets/Sprites/Samurai/Samuraiataque/Samuraiataque1.png");
            player->attackTextures[1] = LoadTexture("assets/Sprites/Samurai/Samuraiataque/Samuraiataque2.png");
            break;
            
        case CHAR_MORDECAI:
            player->speed = 5.5f;
            player->maxHealth = 150;
            
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

// MODIFICADO: Retorna 'bool'
bool UpdatePlayer(Player *player, int screenWidth, int screenHeight, SceneNode* currentScene, bool isPlayer2) {

    if (!player->isAlive) return false; // MODIFICADO: Retorna false

    if (player->collisionDamageTimer > 0) {
        player->collisionDamageTimer -= GetFrameTime();
    }

    int playerOwner = isPlayer2 ? 2 : 1;

    if (player->isReloading) {
        player->reloadTimer -= GetFrameTime();
        if (player->reloadTimer <= 0.0f) {
            ReloadAmmo(playerOwner); 
            player->isReloading = false;
        }
    }

    bool attackPressed = (!isPlayer2 && IsKeyPressed(KEY_X)) || (isPlayer2 && IsKeyPressed(KEY_K));
    if (attackPressed && !player->isAttacking && !player->isReloading) {
        player->isAttacking = true;
        player->currentFrame = 0;
        player->framesCounter = 0;
        player->framesSpeed = player->attackAnimSpeed;
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


        if (player->charType == CHAR_JOHNNY) { 
            SpawnBullet(startPos, player->direction, playerOwner); 
        } 
        else {
            if (player->attackSound.frameCount > 0) { 
                PlaySound(player->attackSound);
            }
        }
    }

    bool reloadPressed = (!isPlayer2 && IsKeyPressed(KEY_R)) || (isPlayer2 && IsKeyPressed(KEY_J));
    if (reloadPressed && player->charType == CHAR_JOHNNY) {
        if (!player->isReloading && GetCurrentAmmo(playerOwner) < 10) { 
            player->isReloading = true;
            player->reloadTimer = 3.0f; 
        }
    }

    bool wasMoving = player->isMoving;
    player->isMoving = false;

    if (!player->isAttacking) { 
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

    else {
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

    int damageTaken = 0;
    Rectangle playerRect = GetPlayerRect(player);
    
    if (CheckEnemyBulletCollision(playerRect, &damageTaken)) { 
        player->health -= damageTaken;
    }

    Texture2D currentTexture = GetPlayerCurrentTexture(player);
    float playerWidth = (float)currentTexture.width * player->scale;
    float playerHeight = (float)currentTexture.height * player->scale;

    float limiteDireito = (float)screenWidth; 
    if (currentScene != NULL && currentScene->id == 5) {
        limiteDireito = 1300.0f;
    }

    if (player->position.x + playerWidth > limiteDireito) {
        if (currentScene != NULL && currentScene->id == 5) {
            player->position.x = limiteDireito - playerWidth;
        } 
        else {
            SceneNode* current = GetCurrentScene();
            if (current->next != NULL) {
                
                // MODIFICADO: Agora P1 e P2 podem ativar a mudança
                // E apenas sinaliza, não executa a mudança
                if (AreAllEnemiesDefeated() && !ControleSpawn_EstaAtivo()) { 
                    return true; // Sinaliza que quer mudar de cena
                } else {
                    player->position.x = screenWidth - playerWidth;
                }
            } else {
                player->position.x = screenWidth - playerWidth;
            }
        }
    }
    else if (player->position.x < 0) {
            player->position.x = 0;
        
    }

    float limiteInferior = (float)screenHeight; 

    if (currentScene != NULL && currentScene->id == 5) {
        limiteInferior = 850.0f; 
    }
    if (player->position.y < RUA_LIMITE_SUPERIOR) {
        player->position.y = RUA_LIMITE_SUPERIOR;
    }
    else if (player->position.y + playerHeight > limiteInferior) {
        player->position.y = limiteInferior - playerHeight;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        player->health += 20;
        if (player->health < 0) player->health = 0;
    }

    return false; // MODIFICADO: Retorno padrão
}

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
        sourceRec.width = -sourceRec.width;
    }
    Rectangle destRec = {
        player->position.x, player->position.y,
        (float)textureToDraw.width * player->scale,
        (float)textureToDraw.height * player->scale
    };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(textureToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
}
    
void DrawPlayerHealthBar(const Player *player, bool isPlayer2) {
    int barWidth = 200;
    int barHeight = 20;
    int barX;
    int barY = 20;

    if (!isPlayer2) {
        barX = 20;
    } else {
        barX = GetScreenWidth() - barWidth - 20;
    }

    float healthPercentage = (float)player->health / (float)player->maxHealth;
    int currentHealthWidth = (int)(barWidth * healthPercentage);
    
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, currentHealthWidth, barHeight, GREEN);
    DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);
}


Texture2D GetPlayerCurrentTexture(const Player *player) {
    if (player->isAttacking) {
        return player->attackTextures[player->currentFrame];
    } else if (player->isMoving) {
        return player->walkTextures[player->currentFrame];
    } else {
        return player->idleTextures[player->currentFrame];
    }
}

Rectangle GetPlayerRect(const Player *player) {
    Texture2D baseTexture = player->idleTextures[0];

    Rectangle rect = {
        player->position.x, player->position.y,
        (float)baseTexture.width * player->scale,
        (float)baseTexture.height * player->scale
    };
    return rect;
}


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

    meleeRect.y = playerRect.y + (playerRect.height / 2) - (MELEE_HEIGHT / 2);
    meleeRect.width = MELEE_RANGE;
    meleeRect.height = MELEE_HEIGHT;

    if (player->direction == 1) {
        meleeRect.x = playerRect.x + playerRect.width;
    } else { 
        meleeRect.x = playerRect.x - MELEE_RANGE;
    }

    return meleeRect;
}


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

    // MODIFICADO: Descarrega o som de ataque se ele foi carregado
    if (player->attackSound.frameCount > 0) {
        UnloadSound(player->attackSound);
    }
}
bool Player_IsReloading(const Player *player) {
    return player->isReloading;
}

void Player_RecebeDano(Player *player, int dano) {
    if (player == NULL) return; 
    if (player->collisionDamageTimer > 0) {
        return; 
    }

    player->health -= dano;
    if (player->health < 0) {
        player->health = 0;
    }
    player->collisionDamageTimer = 0.5f; 
}