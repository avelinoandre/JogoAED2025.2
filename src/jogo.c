#include "jogo.h"
#include "player.h"
#include "raylib.h"
#include "bullet.h"
#include "enemy.h"

static Player player;
static const int screenWidth = 1600;
static const int screenHeight = 900;

static float enemySpawnTimer = 0.0f;

void InitGame(void) {
    InitPlayer(&player, screenWidth / 2, screenHeight / 2);
    
    InitEnemyPool();
    enemySpawnTimer = 0.0f;
}

int UpdateGame(void) {
    UpdatePlayer(&player, screenWidth, screenHeight);
    UpdateEnemyPool(&player);
    
    enemySpawnTimer += GetFrameTime();
    if (enemySpawnTimer >= 3.0f) {
        SpawnEnemy((Vector2){ 100, 100 });
        enemySpawnTimer = 0.0f;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        return 1;
    }
    return 0;
}

void DrawGame(void) {
    ClearBackground(DARKGRAY);
    DrawPlayer(&player);
    
    DrawEnemyPool();
    
    DrawPlayerHealthBar(&player);
    DrawAmmoCount(); 
}

void UnloadGame(void) {
    UnloadPlayer(&player);
    UnloadEnemyAssets();
}