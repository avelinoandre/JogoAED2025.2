#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"
#include "globals.h"
#include "mapa.h"

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
    
    bool isAlive; 
    CharacterType charType; 

    Texture2D *idleTextures;
    Texture2D *walkTextures;
    Texture2D *attackTextures; 

    int idleFrameCount;
    int walkFrameCount;
    int attackFrameCount;
    
    int attackAnimSpeed;

    Sound attackSound;

    int currentFrame;
    int framesCounter;
    int framesSpeed; 

    bool isReloading;       
    float reloadTimer;

    float collisionDamageTimer;
} Player;


void InitPlayer(Player *player, CharacterType charType, int startX, int startY);
void UpdatePlayer(Player *player, int screenWidth, int screenHeight, SceneNode* currentScene, bool isPlayer2);
void DrawPlayer(const Player *player);
void DrawPlayerHealthBar(const Player *player, bool isPlayer2);
void UnloadPlayer(Player *player);
Texture2D GetPlayerCurrentTexture(const Player *player);
void SpawnBullet(Vector2 startPos, int direction, int playerOwner);
Rectangle GetPlayerRect(const Player *player);
Rectangle GetPlayerMeleeRect(Player *player);
void Player_RecebeDano(Player *player, int dano);
bool Player_IsReloading(const Player *player);

#endif