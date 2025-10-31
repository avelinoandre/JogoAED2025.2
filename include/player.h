#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"
#include "animation.h"

typedef struct Player {
    Vector2 position;
    float speed;
    int maxHealth;
    int health;
    float scale;
    int direction;
    bool isMoving;

    Texture2D atlas;       
    SpriteAnimation animIdle; 
    SpriteAnimation animWalk; 

    float frameWidth;
    float frameHeight;

} Player;

void InitPlayer(Player *player, int startX, int startY);
void UpdatePlayer(Player *player, int screenWidth, int screenHeight);
void DrawPlayer(const Player *player);
void DrawPlayerHealthBar(const Player *player);
void UnloadPlayer(Player *player);

#endif