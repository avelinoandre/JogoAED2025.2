#include "jogo.h"
#include "player.h"
#include "raylib.h"
#include "bullet.h"
#include "enemy.h"
#include <stdlib.h> 
#include <time.h>

static Player player;
static const int screenWidth = 1600;
static const int screenHeight = 900;

static float enemySpawnTimer = 0.0f;

void InitGame(void) {
    srand(time(NULL));
    InitPlayer(&player, screenWidth / 2, screenHeight / 2);
    
    InitEnemyPool();
    InitBulletPool();
    enemySpawnTimer = 0.0f;
}

int UpdateGame(void) {
    UpdatePlayer(&player, screenWidth, screenHeight);
    UpdateBulletPool(screenWidth, screenHeight);
    UpdateEnemyPool(&player);
    
    enemySpawnTimer += GetFrameTime();
    if (enemySpawnTimer >= 3.0f) {
        SpawnEnemy((Vector2){ 100, 100 });
        enemySpawnTimer = 0.0f;
    }

    AmmoPack* pack = GetAmmoPack();
    if (IsReloading() && !pack->active) {
        
        int randX = rand() % (screenWidth - 50); 
        int randY = rand() % (screenHeight - 50);

        SpawnAmmoPack((Vector2){ (float)randX, (float)randY });
    }
    if (pack->active) { 
    
        Texture2D currentTexture = GetPlayerCurrentTexture(&player);
        Rectangle playerBounds = {
            player.position.x,
            player.position.y,
            (float)currentTexture.width * player.scale,
            (float)currentTexture.height * player.scale
        };

        Texture2D ammoTexture = GetAmmoPackTexture();
        Rectangle ammoBounds = {
            pack->position.x,
            pack->position.y,
            (float)ammoTexture.width * 4.0f,  // <-- MUDE AQUI
            (float)ammoTexture.height * 4.0f // <-- E AQUI
        };

        if (CheckCollisionRecs(playerBounds, ammoBounds)) {
            ReloadAmmo();
        }
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

    DrawBulletPool(); 
    DrawAmmoPack();
    
    DrawPlayerHealthBar(&player);
    DrawAmmoCount(); 
}

void UnloadGame(void) {
    UnloadPlayer(&player);
    UnloadEnemyAssets();
    UnloadBulletAssets();
}