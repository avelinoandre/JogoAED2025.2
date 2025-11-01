#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"

#define PLAYER_IDLE_FRAMES 4
#define PLAYER_WALK_FRAMES 5

typedef struct Player {
    Vector2 position;
    float speed;
    int maxHealth;
    int health;
    float scale;
    int direction;
    bool isMoving;

    Texture2D idleTextures[PLAYER_IDLE_FRAMES];
    Texture2D walkTextures[PLAYER_WALK_FRAMES];

    int currentFrame;
    int framesCounter;
    int framesSpeed;

} Player;

void InitPlayer(Player *player, int startX, int startY);
void UpdatePlayer(Player *player, int screenWidth, int screenHeight);
void DrawPlayer(const Player *player);
void DrawPlayerHealthBar(const Player *player);
void UnloadPlayer(Player *player);

#endif