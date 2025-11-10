#ifndef ENEMY_H_
#define ENEMY_H_

#include "raylib.h"
#include "player.h"

#define MAX_ENEMIES 10
#define ENEMY_SPEED 1.5f
#define ENEMY_HEALTH 100
#define ENEMY_SCALE 4.0f

#define ENEMY_DAMAGE 1
#define ENEMY_ATTACK_COOLDOWN 1.0f 

typedef struct Enemy {
    Vector2 position;
    int health;
    bool active;
    float attackTimer;
} Enemy;

void InitEnemyPool(void);
void UnloadEnemyAssets(void);
void UpdateEnemyPool(Player *player);
void DrawEnemyPool(void);
void SpawnEnemy(Vector2 position);

#endif