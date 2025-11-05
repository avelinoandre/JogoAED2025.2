#include "enemy.h"
#include "raymath.h" 
#include <math.h>    

static Enemy enemyPool[MAX_ENEMIES];
static Texture2D enemyTexture;

void InitEnemyPool(void) {
    enemyTexture = LoadTexture("assets/Sprites/Garnet/Garnetparada/Garnet1.png");

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyPool[i].active = false;
    }
}

void UnloadEnemyAssets(void) {
    UnloadTexture(enemyTexture);
}

void SpawnEnemy(Vector2 position) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemyPool[i].active) {
            enemyPool[i].active = true;
            enemyPool[i].position = position;
            enemyPool[i].health = ENEMY_HEALTH;
            return;
        }
    }
}

void UpdateEnemyPool(Player *player) {
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemyPool[i].active) continue;
        
        Enemy *enemy = &enemyPool[i];

        if (Vector2Distance(enemy->position, player->position) > 50.0f) 
        {

            Vector2 direction = Vector2Subtract(player->position, enemy->position);
            direction = Vector2Normalize(direction);

            enemy->position = Vector2Add(enemy->position, Vector2Scale(direction, ENEMY_SPEED));
        }

        if (enemy->health <= 0) {
            enemy->active = false; 
        }
    }
}

void DrawEnemyPool(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemyPool[i].active) {
            DrawTextureEx(
                enemyTexture,
                enemyPool[i].position,
                0.0f,
                ENEMY_SCALE,
                WHITE
            );
            
            DrawRectangle(
                (int)enemyPool[i].position.x, 
                (int)enemyPool[i].position.y - 10, // 10 pixels acima do sprite
                (int)(enemyTexture.width * ENEMY_SCALE * ((float)enemyPool[i].health / ENEMY_HEALTH)), 
                5, // 5 pixels de altura
                RED
            );
        }
    }
}