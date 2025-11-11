#include "enemy.h"
#include "raymath.h"
#include <math.h>
#include "bullet.h"   
#include <stdlib.h>  
#include <stdio.h>   
#include "globals.h"  

#define ENEMY_ANIM_SPEED_PARADO 50
#define ENEMY_ANIM_SPEED_ANDANDO 20
#define ENEMY_MELEE_DAMAGE 50

static Enemy enemyPool[MAX_ENEMIES];


#define GARNET_IDLE_FRAMES 2
#define GARNET_WALK_FRAMES 4
#define GARNET_ATTACK_FRAMES 2
#define GARNET_ATTACK_SPEED 15 
static Texture2D garnetIdleTextures[GARNET_IDLE_FRAMES];
static Texture2D garnetWalkTextures[GARNET_WALK_FRAMES];
static Texture2D garnetAttackTextures[GARNET_ATTACK_FRAMES];

#define Limao_IDLE_FRAMES 2
#define Limao_WALK_FRAMES 4
#define Limao_ATTACK_FRAMES 2
#define Limao_ATTACK_SPEED 10
static Texture2D LimaoIdleTextures[Limao_IDLE_FRAMES];
static Texture2D LimaoWalkTextures[Limao_WALK_FRAMES];
static Texture2D LimaoAttackTextures[Limao_ATTACK_FRAMES];

#define Mojo_IDLE_FRAMES 2
#define Mojo_WALK_FRAMES 2
#define Mojo_ATTACK_FRAMES 1
#define Mojo_ATTACK_SPEED 10
static Texture2D MojoIdleTextures[Mojo_IDLE_FRAMES];
static Texture2D MojoWalkTextures[Mojo_WALK_FRAMES];
static Texture2D MojoAttackTextures[Mojo_ATTACK_FRAMES];

#define Marvin_IDLE_FRAMES 2
#define Marvin_WALK_FRAMES 4
#define Marvin_ATTACK_FRAMES 2
#define Marvin_ATTACK_SPEED 10
static Texture2D MarvinIdleTextures[Marvin_IDLE_FRAMES];
static Texture2D MarvinWalkTextures[Marvin_WALK_FRAMES];
static Texture2D MarvinAttackTextures[Marvin_ATTACK_FRAMES];


Texture2D GetEnemyCurrentTexture(const Enemy *enemy) {
    
    if (enemy->isAttacking && enemy->attackTextures != NULL) {
        int frame = enemy->currentFrame;
        if (frame >= enemy->attackFrameCount) frame = enemy->attackFrameCount - 1;
        return enemy->attackTextures[frame];
        
    } else if (enemy->isMoving && enemy->walkTextures != NULL) {
        int frame = enemy->currentFrame;
        if (frame >= enemy->walkFrameCount) frame = enemy->walkFrameCount - 1;
        return enemy->walkTextures[frame];
        
    } else if (enemy->idleTextures != NULL) {
        int frame = enemy->currentFrame;
        if (frame >= enemy->idleFrameCount) frame = enemy->idleFrameCount - 1;
        return enemy->idleTextures[frame];
    }
    return (Texture2D){ 0 };
}

Rectangle GetEnemyRect(const Enemy *enemy) {
    Texture2D currentTexture = GetEnemyCurrentTexture(enemy);
    if (currentTexture.id <= 0) return (Rectangle){0, 0, 0, 0};
    
    Rectangle rect = {
        enemy->position.x, enemy->position.y,
        (float)currentTexture.width * enemy->scale,
        (float)currentTexture.height * enemy->scale
    };
    return rect;
}

void InitEnemyPool(void) {
    char path[256]; 

    for (int i = 0; i < GARNET_IDLE_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Garnet/Garnetparada/Garnet_parada%d.png", i + 1);
        garnetIdleTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < GARNET_WALK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Garnet/Garnetmovimentacao/Garnet_andando%d.png", i + 1);
        garnetWalkTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < GARNET_ATTACK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Garnet/Garnetataque/Garnet_ataque%d.png", i + 1);
        garnetAttackTextures[i] = LoadTexture(path);
    }


    for (int i = 0; i < Limao_IDLE_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Limao/Limaoparado/Limaoparado%d.png", i + 1);
        LimaoIdleTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < Limao_WALK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Limao/Limaoandando/Limaoandando%d.png", i + 1);
        LimaoWalkTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < Limao_ATTACK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Limao/Limaoataque/Limaoataque%d.png", i + 1);
        LimaoAttackTextures[i] = LoadTexture(path);
    }
    

    for (int i = 0; i < Mojo_IDLE_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Mojo/Mojoparado/Mojoparado%d.png", i + 1);
        MojoIdleTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < Mojo_WALK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Mojo/Mojoandando/Mojoandando%d.png", i + 1);
        MojoWalkTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < Mojo_ATTACK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Mojo/Mojoataque/Mojoataque%d.png", i + 1);
        MojoAttackTextures[i] = LoadTexture(path);
    }

    
    for (int i = 0; i < Marvin_IDLE_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Marvin/Marvinparado/Marvinparado%d.png", i + 1);
        MarvinIdleTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < Marvin_WALK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Marvin/Marvinandando/marvinandando%d.png", i + 1);
        MarvinWalkTextures[i] = LoadTexture(path);
    }
    for (int i = 0; i < Marvin_ATTACK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/Marvin/Marvinataque/Marvinataque%d.png", i + 1);
        MarvinAttackTextures[i] = LoadTexture(path);
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyPool[i].active = false;
        enemyPool[i].idleTextures = NULL; 
        enemyPool[i].walkTextures = NULL;
        enemyPool[i].attackTextures = NULL;
    }
}

void UnloadEnemyAssets(void) {
    
    for (int i = 0; i < GARNET_IDLE_FRAMES; i++) {
        UnloadTexture(garnetIdleTextures[i]);
    }
    for (int i = 0; i < GARNET_WALK_FRAMES; i++) {
        UnloadTexture(garnetWalkTextures[i]);
    }
    for (int i = 0; i < GARNET_ATTACK_FRAMES; i++) {
        UnloadTexture(garnetAttackTextures[i]);
    }

    
    for (int i = 0; i < Limao_IDLE_FRAMES; i++) {
        UnloadTexture(LimaoIdleTextures[i]);
    }
    for (int i = 0; i < Limao_WALK_FRAMES; i++) {
        UnloadTexture(LimaoWalkTextures[i]);
    }
    for (int i = 0; i < Limao_ATTACK_FRAMES; i++) {
        UnloadTexture(LimaoAttackTextures[i]);
    }

    
    for (int i = 0; i < Mojo_IDLE_FRAMES; i++) {
        UnloadTexture(MojoIdleTextures[i]);
    }
    for (int i = 0; i < Mojo_WALK_FRAMES; i++) {
        UnloadTexture(MojoWalkTextures[i]);
    }
    for (int i = 0; i < Mojo_ATTACK_FRAMES; i++) {
        UnloadTexture(MojoAttackTextures[i]);
    }

    
    for (int i = 0; i < Marvin_IDLE_FRAMES; i++) {
        UnloadTexture(MarvinIdleTextures[i]);
    }
    for (int i = 0; i < Marvin_WALK_FRAMES; i++) {
        UnloadTexture(MarvinWalkTextures[i]);
    }
    for (int i = 0; i < Marvin_ATTACK_FRAMES; i++) {
        UnloadTexture(MarvinAttackTextures[i]);
    }
}

void SpawnEnemy(EnemyType type, Vector2 position) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemyPool[i].active) {
            Enemy *enemy = &enemyPool[i]; 
            
            enemy->active = true;
            enemy->position = position;
            enemy->type = type;
            enemy->direction = -1; 
            enemy->currentFrame = 0;
            enemy->framesCounter = 0;
            enemy->attackTimer = 0;
            enemy->isAttacking = false;
            enemy->isMoving = false;
            
            switch (type) {
                
                case ENEMY_GARNET: {
                    enemy->health = 100;
                    enemy->maxHealth = 100;
                    enemy->speed = 1.8f;
                    enemy->scale = 4.0f;
                    enemy->damage = 10;
                    enemy->attackCooldown = 1.2f; 
                    enemy->framesSpeed = GARNET_ATTACK_SPEED;

                    enemy->idleFrameCount = GARNET_IDLE_FRAMES;
                    enemy->walkFrameCount = GARNET_WALK_FRAMES;
                    enemy->attackFrameCount = GARNET_ATTACK_FRAMES;
                    
                    enemy->idleTextures = garnetIdleTextures;
                    enemy->walkTextures = garnetWalkTextures;
                    enemy->attackTextures = garnetAttackTextures;
                    break;
                }

                case ENEMY_LIMAO: { 
                    enemy->health = 60;
                    enemy->maxHealth = 60;
                    enemy->speed = 2.5f; 
                    enemy->scale = 3.5f; 
                    enemy->damage = 5;   
                    enemy->attackCooldown = 0.8f; 
                    enemy->framesSpeed = Limao_ATTACK_SPEED;

                    enemy->idleFrameCount = Limao_IDLE_FRAMES;
                    enemy->walkFrameCount = Limao_WALK_FRAMES;
                    enemy->attackFrameCount = Limao_ATTACK_FRAMES;
                    
                    enemy->idleTextures = LimaoIdleTextures;
                    enemy->walkTextures = LimaoWalkTextures;
                    enemy->attackTextures = LimaoAttackTextures;
                    break;
                }
                    
                case ENEMY_MOJO: { 
                    enemy->health = 200;
                    enemy->maxHealth = 200;
                    enemy->speed = 1.0f; 
                    enemy->scale = 4.5f; 
                    enemy->damage = 20;  
                    enemy->attackCooldown = 2.1f; 
                    enemy->framesSpeed = Mojo_ATTACK_SPEED;

                    enemy->idleFrameCount = Mojo_IDLE_FRAMES;
                    enemy->walkFrameCount = Mojo_WALK_FRAMES;
                    enemy->attackFrameCount = Mojo_ATTACK_FRAMES;
                    
                    enemy->idleTextures = MojoIdleTextures;
                    enemy->walkTextures = MojoWalkTextures;
                    enemy->attackTextures = MojoAttackTextures;
                    break;
                }

                case ENEMY_MARVIN: { 
                    enemy->health = 80;
                    enemy->maxHealth = 80;
                    enemy->speed = 1.5f; 
                    enemy->scale = 4.0f;
                    enemy->damage = 15;  
                    enemy->attackCooldown = 1.8f; 
                    enemy->framesSpeed = Marvin_ATTACK_SPEED;

                    enemy->idleFrameCount = Marvin_IDLE_FRAMES;
                    enemy->walkFrameCount = Marvin_WALK_FRAMES;
                    enemy->attackFrameCount = Marvin_ATTACK_FRAMES;
                    
                    enemy->idleTextures = MarvinIdleTextures;
                    enemy->walkTextures = MarvinWalkTextures;
                    enemy->attackTextures = MarvinAttackTextures;
                    break;
                }

                default: {
                    enemy->active = false;
                    break;
                }
            }
            return;
        }
    }
}

void DespawnAllEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyPool[i].active = false;
    }
}

void UpdateEnemyPool(Player *player, int screenHeight) {
    
    Rectangle playerRect = GetPlayerRect(player);
    Rectangle meleeRect = GetPlayerMeleeRect(player); 

    for (int i = 0; i < MAX_ENEMIES; i++) {
        
        if (!enemyPool[i].active) {
            continue;
        }
        
        Enemy *enemy = &enemyPool[i];
        bool wasMoving = enemy->isMoving;
        enemy->isMoving = false; 

        if (enemy->attackTimer > 0) {
            enemy->attackTimer -= GetFrameTime();
        }

        if (!enemy->isAttacking) {
            
            Vector2 playerPos = player->position;
            
            float distanceX = fabsf(playerPos.x - enemy->position.x);
            float distanceY = fabsf(playerPos.y - enemy->position.y);
            float alignThreshold = 20.0f; 

            
            switch(enemy->type) {
                
                case ENEMY_GARNET:
                case ENEMY_LIMAO:
                {
                    float distance = Vector2Distance(playerPos, enemy->position);

                    if (distance > 100.0f) { 
                        Vector2 direction = Vector2Normalize(Vector2Subtract(playerPos, enemy->position)); 
                        enemy->position = Vector2Add(enemy->position, Vector2Scale(direction, enemy->speed));
                        enemy->isMoving = true;
                    } else if (enemy->attackTimer <= 0) { 
                        enemy->isAttacking = true;
                        enemy->currentFrame = 0;
                        enemy->framesCounter = 0;
                        enemy->attackTimer = enemy->attackCooldown;
                    }
                    break;
                }
                
                case ENEMY_MOJO:
                {
                    float minRange = 300.0f; 
                    float maxRange = 350.0f; 

                    bool isAlignedY = (distanceY <= alignThreshold);
                    bool canAttack = (enemy->attackTimer <= 0);
                    bool inShootRange = (distanceX <= maxRange);

                    
                    if (canAttack && isAlignedY && inShootRange) 
                    {
                        enemy->isAttacking = true;
                        enemy->currentFrame = 0;
                        enemy->framesCounter = 0;
                        enemy->attackTimer = enemy->attackCooldown;
                        
                        Rectangle enemyRect = GetEnemyRect(enemy);
                        Vector2 spawnPos;
                        spawnPos.y = enemy->position.y + (enemyRect.height * 0.6f); 

                        if (enemy->direction == -1) { 
                            spawnPos.x = enemy->position.x + (enemyRect.width * 0.2f); 
                        } else { 
                            spawnPos.x = enemy->position.x + (enemyRect.width * 0.8f);
                        }
                        
                        SpawnEnemyBullet(spawnPos, enemy->direction, ENEMY_BULLET_SPEED_MOJO, enemy->damage);
                    } 
                    else 
                    {
                        if (!isAlignedY) {
                            if (playerPos.y < enemy->position.y) {
                                enemy->position.y -= enemy->speed * 0.7f;
                            } else {
                                enemy->position.y += enemy->speed * 0.7f;
                            }
                            enemy->isMoving = true;
                        }

                        if (distanceX < minRange) { 
                            if (playerPos.x < enemy->position.x) {
                                enemy->position.x += enemy->speed * 0.8f; 
                            } else {
                                enemy->position.x -= enemy->speed * 0.8f;
                            }
                            enemy->isMoving = true;
                        } 
                        else if (distanceX > maxRange) { 
                            if (playerPos.x < enemy->position.x) {
                                enemy->position.x -= enemy->speed;
                            } else {
                                enemy->position.x += enemy->speed;
                            }
                            enemy->isMoving = true;
                        }
                    }
                    break;
                }

                case ENEMY_MARVIN: 
                {
                    float minRange = 450.0f;
                    float maxRange = 500.0f;
                    
                    bool isAlignedY = (distanceY <= alignThreshold);
                    bool canAttack = (enemy->attackTimer <= 0);
                    bool inShootRange = (distanceX <= maxRange); 

                    if (canAttack && isAlignedY && inShootRange)
                    {
                        enemy->isAttacking = true;
                        enemy->currentFrame = 0;
                        enemy->framesCounter = 0;
                        enemy->attackTimer = enemy->attackCooldown;

                        Rectangle enemyRect = GetEnemyRect(enemy);
                        Vector2 spawnPos;
                        spawnPos.y = enemy->position.y + (enemyRect.height * 0.55f); 

                        if (enemy->direction == -1) { 
                            spawnPos.x = enemy->position.x + (enemyRect.width * 0.1f);
                        } else { 
                            spawnPos.x = enemy->position.x + (enemyRect.width * 0.9f);
                        }

                        SpawnEnemyBullet(spawnPos, enemy->direction, ENEMY_BULLET_SPEED_MARVIN, enemy->damage);
                    } 
                    else 
                    {
                        if (!isAlignedY) {
                            if (playerPos.y < enemy->position.y) {
                                enemy->position.y -= enemy->speed * 0.7f;
                            } else {
                                enemy->position.y += enemy->speed * 0.7f;
                            }
                            enemy->isMoving = true;
                        }

                        if (distanceX < minRange) { 
                            if (playerPos.x < enemy->position.x) {
                                enemy->position.x += enemy->speed * 0.8f;
                            } else {
                                enemy->position.x -= enemy->speed * 0.8f;
                            }
                            enemy->isMoving = true;
                        } 
                        else if (distanceX > maxRange) { 
                            if (playerPos.x < enemy->position.x) {
                                enemy->position.x -= enemy->speed;
                            } else {
                                enemy->position.x += enemy->speed;
                            }
                            enemy->isMoving = true;
                        }
                    }
                    break; 
                }
            }
            
            if (playerPos.x < enemy->position.x) {
                enemy->direction = -1;
            }
            if (playerPos.x > enemy->position.x) {
                enemy->direction = 1;
            }
        }
        
        int currentAnimFrameCount = 0;
        
        if (enemy->isAttacking) {
            currentAnimFrameCount = enemy->attackFrameCount; 
            enemy->framesCounter++;
            
            if (enemy->framesCounter >= enemy->framesSpeed) {
                enemy->framesCounter = 0;
                enemy->currentFrame++;
                
                if (enemy->currentFrame >= (enemy->attackFrameCount - 1)) {
                    if (enemy->type == ENEMY_GARNET || enemy->type == ENEMY_LIMAO) {
                        if (CheckCollisionRecs(GetEnemyRect(enemy), playerRect)) {
                            player->health -= enemy->damage; 
                        }
                    }
                }
                
                if (enemy->currentFrame >= currentAnimFrameCount) {
                    enemy->isAttacking = false; 
                    enemy->currentFrame = 0;
                }
            }
        } 
        else { 
            if (enemy->isMoving) {
                currentAnimFrameCount = enemy->walkFrameCount;
            } else {
                currentAnimFrameCount = enemy->idleFrameCount;
            }
            
            if (enemy->isMoving != wasMoving) { 
                enemy->currentFrame = 0;
                enemy->framesCounter = 0;
                enemy->framesSpeed = enemy->isMoving ? ENEMY_ANIM_SPEED_ANDANDO : ENEMY_ANIM_SPEED_PARADO;
            }
            
            enemy->framesCounter++;
            
            if (enemy->framesCounter >= enemy->framesSpeed) {
                enemy->framesCounter = 0;
                enemy->currentFrame++;
                if (enemy->currentFrame >= currentAnimFrameCount) {
                    enemy->currentFrame = 0;
                }
            }
        }
        
        Rectangle enemyRect = GetEnemyRect(enemy);
        int screenWidth = GetScreenWidth(); 

        if (enemy->position.y < RUA_LIMITE_SUPERIOR) {
            enemy->position.y = RUA_LIMITE_SUPERIOR;
        }
        if (enemy->position.y + enemyRect.height > screenHeight) {
            enemy->position.y = screenHeight - enemyRect.height;
        }

        if (enemy->position.x < 0) {
            enemy->position.x = 0;
        }
        if (enemy->position.x + enemyRect.width > screenWidth) {
            enemy->position.x = screenWidth - enemyRect.width;
        }
        
        if (player->collisionDamageTimer <= 0 && CheckCollisionRecs(playerRect, enemyRect)) {
            player->health -= 3; 
            player->collisionDamageTimer = 0.5f; 
        }

        int damageTaken = 0;
        if (selectedCharacter == CHAR_JOHNNY) {
            if (CheckBulletCollision(enemyRect, &damageTaken)) {
                enemy->health -= damageTaken;
            }
        } else {
            if (player->isAttacking && meleeRect.width > 0 && CheckCollisionRecs(enemyRect, meleeRect)) {
                if(enemy->attackTimer <= 0.1f) { 
                    enemy->health -= ENEMY_MELEE_DAMAGE; 
                    enemy->attackTimer = 0.5f; 
                    enemy->isAttacking = false; 
                }
            }
        }

        if (enemy->health <= 0) {
            enemy->active = false; 
        }
    }
}

void DrawEnemyPool(void) {
    
    #define ENEMY_HEALTHBAR_WIDTH 80 

    for (int i = 0; i < MAX_ENEMIES; i++) {
        
        if (enemyPool[i].active) {
            Enemy *enemy = &enemyPool[i];
            
            Texture2D textureToDraw = GetEnemyCurrentTexture(enemy);
            
            if (textureToDraw.id <= 0) {
                continue; 
            }

            Rectangle sourceRec = { 0.0f, 0.0f, (float)textureToDraw.width, (float)textureToDraw.height };
            
            if (enemy->direction == -1) { 
                sourceRec.width = -sourceRec.width;
            }

            Rectangle destRec = {
                enemy->position.x, enemy->position.y,
                (float)textureToDraw.width * enemy->scale,
                (float)textureToDraw.height * enemy->scale
            };
            Vector2 origin = { 0.0f, 0.0f };
            
            DrawTexturePro(textureToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
            
           
            float healthPercent = (float)enemy->health / (float)enemy->maxHealth;
            int barWidth = (int)(ENEMY_HEALTHBAR_WIDTH * healthPercent);
            int barX = (int)enemy->position.x + (int)((destRec.width - ENEMY_HEALTHBAR_WIDTH) / 2); // Centraliza a barra
            int barY = (int)enemy->position.y - 10;
            
            DrawRectangle(barX, barY, ENEMY_HEALTHBAR_WIDTH, 5, DARKGRAY);
            DrawRectangle(barX, barY, barWidth, 5, RED);

        }
    }
}

bool AreAllEnemiesDefeated(void) {
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        
        if (enemyPool[i].active) {
            return false;
        }
    }
    
    return true;
}

int GetActiveEnemyCount(void) {
    int count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemyPool[i].active) {
            count++;
        }
    }
    return count;
}