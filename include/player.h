#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"
#include "mapa.h" 
#include "globals.h" 

typedef struct Player {
    Vector2 position;
    float scale;
    float speed;
    int direction; 
    bool isMoving;
    bool isAttacking;
    
    int health;
    int maxHealth;
    bool isAlive;
    
    CharacterType charType;

    Texture2D *idleTextures;
    Texture2D *walkTextures;
    Texture2D *attackTextures;
    
    int idleFrameCount;
    int walkFrameCount;
    int attackFrameCount;
    
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    int attackAnimSpeed;

    bool isReloading;
    float reloadTimer;

    Sound attackSound;

    float collisionDamageTimer; 

} Player;

void InitPlayer(Player *player, CharacterType charType, int startX, int startY);

bool UpdatePlayer(Player *player, int screenWidth, int screenHeight, SceneNode* currentScene, bool isPlayer2);

void DrawPlayer(const Player *player);
void DrawPlayerHealthBar(const Player *player, bool isPlayer2);
Texture2D GetPlayerCurrentTexture(const Player *player);
Rectangle GetPlayerRect(const Player *player);
Rectangle GetPlayerMeleeRect(Player *player); 
void UnloadPlayer(Player *player);

bool Player_IsReloading(const Player *player);
void Player_RecebeDano(Player *player, int dano);

#endif 