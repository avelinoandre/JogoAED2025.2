#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct Player {
    Vector2 position;
    float speed;
    int health;
    int maxHealth;
    
    Texture2D texture;
    float scale;
    int currentFrame;
    int framesCounter;
    int direction;
    bool isMoving;

} Player;

void InitPlayer(Player *player, int startX, int startY);
void UpdatePlayer(Player *player, int screenWidth, int screenHeight);
void DrawPlayer(const Player *player);
void DrawPlayerHealthBar(const Player *player);
void UnloadPlayer(Player *player);

#endif