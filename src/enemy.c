#include "enemy.h"
#include "raymath.h" 
#include <math.h>    
#include "bullet.h"

static Enemy enemyPool[MAX_ENEMIES];
static Texture2D enemyTexture;

void InitEnemyPool(void) {
    enemyTexture = LoadTexture("assets/Sprites/Garnet/Garnetparada/Garnet1.png");

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyPool[i].active = false;
        enemyPool[i].attackTimer = 0;
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
            enemyPool[i].attackTimer = 0;
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

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemyPool[i].active) continue;
        
        Enemy *enemy = &enemyPool[i];

        if (Vector2Distance(enemy->position, player->position) > 50.0f){
            Vector2 direction = Vector2Subtract(player->position, enemy->position);
            direction = Vector2Normalize(direction); 
            enemy->position = Vector2Add(enemy->position, Vector2Scale(direction, ENEMY_SPEED));
        }
        
        float enemyHeight = (float)enemyTexture.height * ENEMY_SCALE;
        if (enemy->position.y < RUA_LIMITE_SUPERIOR) {
            enemy->position.y = RUA_LIMITE_SUPERIOR;
        }
        if (enemy->position.y + enemyHeight > screenHeight) {
            enemy->position.y = screenHeight - enemyHeight;
        }

        Rectangle enemyRect = {
            enemy->position.x,
            enemy->position.y,
            (float)enemyTexture.width * ENEMY_SCALE,
            (float)enemyTexture.height * ENEMY_SCALE
        };

        int damageTaken = 0;
        if (CheckBulletCollision(enemyRect, &damageTaken)) {
            enemy->health -= damageTaken;
        }

        if (enemy->attackTimer > 0) {
            enemy->attackTimer -= GetFrameTime();
        }

        if (CheckCollisionRecs(playerRect, enemyRect) && enemy->attackTimer <= 0) {
            player->health -= ENEMY_DAMAGE;
            enemy->attackTimer = ENEMY_ATTACK_COOLDOWN;
        }

        if (enemy->health <= 0) {
            enemy->active = false; 
        }
    }
}

void DrawEnemyPool(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemyPool[i].active) {
            Color tint = (enemyPool[i].attackTimer > ENEMY_ATTACK_COOLDOWN - 0.1f) ? RED : WHITE;
            DrawTextureEx( 
                enemyTexture,
                enemyPool[i].position,
                0.0f,
                ENEMY_SCALE,
                tint
            );
            DrawRectangle( 
                (int)enemyPool[i].position.x, 
                (int)enemyPool[i].position.y - 10,
                (int)(enemyTexture.width * ENEMY_SCALE * ((float)enemyPool[i].health / ENEMY_HEALTH)), 
                5, 
                RED
            );
        }
    }
}