#include "jogo.h"
#include "player.h" 
#include "raylib.h"
#include "bullet.h"
#include "enemy.h"
#include "mapa.h"
#include <stdlib.h> 
#include <time.h>
#include "globals.h"

CharacterType selectedCharacter = CHAR_JOHNNY;

static Player player;
static const int screenWidth = 1600;
static const int screenHeight = 900;

void SpawnSceneEnemies(SceneNode* scene) {

    if (scene == NULL || scene->enemiesSpawned) {
        return;
    }

    printf("Spawning %d inimigos para a nova cena!\n", scene->enemyCount);
    
    for (int i = 0; i < scene->enemyCount; i++) {
        float spawnX = (float)(screenWidth + 50 + (rand() % 150));
        float spawnY = RUA_LIMITE_SUPERIOR + (rand() % (int)(screenHeight - RUA_LIMITE_SUPERIOR - 150)); 
        
        
        int randomType = rand() % 4;

        SpawnEnemy((EnemyType)randomType, (Vector2){spawnX, spawnY});
        
    }

    scene->enemiesSpawned = true;
}

void InitGame(void) {
    InitMap();

    srand(time(NULL));
    InitPlayer(&player, screenWidth / 2, screenHeight / 2);
    
    InitEnemyPool();
    InitBulletPool();
    InitEnemyBulletPool();

    SpawnSceneEnemies(GetCurrentScene());
}

int UpdateGame(void) {
    if (player.health <= 0) {
        return 2; 
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        return 1; 
    }

    UpdatePlayer(&player, screenWidth, screenHeight);

    if (selectedCharacter == CHAR_JOHNNY) 
    {
        UpdateBulletPool(screenWidth, screenHeight);
        
        AmmoPack* pack = GetAmmoPack();

        if (IsReloading() && !pack->active) {
            
            int randX = rand() % (screenWidth - 50); 
            int alturaJogavel = (int)(screenHeight - RUA_LIMITE_SUPERIOR - 150);
            if (alturaJogavel <= 0) alturaJogavel = 1; 
            int randY = RUA_LIMITE_SUPERIOR + (rand() % alturaJogavel);

            SpawnAmmoPack((Vector2){ (float)randX, (float)randY });
        }

        if (pack->active) { 
            Texture2D currentTexture = GetPlayerCurrentTexture(&player);
            Rectangle playerBounds = GetPlayerRect(&player); 
            Texture2D ammoTexture = GetAmmoPackTexture();
            Rectangle ammoBounds = {
                pack->position.x,
                pack->position.y,
                (float)ammoTexture.width * 4.0f,
                (float)ammoTexture.height * 4.0f
            };
            if (CheckCollisionRecs(playerBounds, ammoBounds)) {
                ReloadAmmo();
            }
        }
    }

    UpdateEnemyPool(&player, screenHeight);
    UpdateEnemyBulletPool(screenWidth, screenHeight);
    
    SpawnSceneEnemies(GetCurrentScene());
    return 0; 
}

void DrawGame(void) {
    ClearBackground(DARKGRAY);

    DrawCurrentMap();
    DrawPlayer(&player);
    
    DrawEnemyPool();
    DrawEnemyBulletPool();

    if (selectedCharacter == CHAR_JOHNNY)
    {
        DrawBulletPool(); 
        DrawAmmoPack();
        DrawAmmoCount(); 
    }

    DrawPlayerHealthBar(&player);
}

void UnloadGame(void) {
    UnloadMap();
    UnloadPlayer(&player);
    UnloadEnemyAssets();
    UnloadBulletAssets();
    UnloadEnemyBulletAssets();
}