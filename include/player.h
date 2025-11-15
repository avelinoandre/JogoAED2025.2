#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"
#include "globals.h"

#define RUA_LIMITE_SUPERIOR 450.0f

typedef struct Player {
    Vector2 position;
    float speed;
    int maxHealth;
    int health;
    float scale;
    int direction;
    
    bool isMoving;
    bool isAttacking;

    Texture2D *idleTextures;
    Texture2D *walkTextures;
    Texture2D *attackTextures; 

    int idleFrameCount;
    int walkFrameCount;
    int attackFrameCount;
    
    int attackAnimSpeed;

    int currentFrame;
    int framesCounter;
    int framesSpeed; 

    bool isReloading;       
    float reloadTimer;

    float collisionDamageTimer;
} Player;

void InitPlayer(Player *player, int startX, int startY);
void UpdatePlayer(Player *player, int screenWidth, int screenHeight);
void DrawPlayer(const Player *player);
void DrawPlayerHealthBar(const Player *player);
void UnloadPlayer(Player *player);
Texture2D GetPlayerCurrentTexture(const Player *player);
void SpawnBullet(Vector2 startPos, int direction);
Rectangle GetPlayerRect(const Player *player);
Rectangle GetPlayerMeleeRect(Player *player);

#endif