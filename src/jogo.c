#include "jogo.h"
#include "player.h" 
#include "raylib.h"
#include "bullet.h"
#include "enemy.h"
#include "mapa.h"
#include <stdlib.h> 
#include <time.h>
#include "globals.h"
#include "gemini_ai.h" 
#include <stdio.h>   

CharacterType selectedCharacter = CHAR_JOHNNY;

static Player player;
static const int screenWidth = 1600;
static const int screenHeight = 900;
static int totalEnemiesInScene = 0;


void InitGame(void) {
    InitMap();

    srand(time(NULL));
    InitPlayer(&player, screenWidth / 2, screenHeight / 2);
    
    InitEnemyPool();
    InitBulletPool();
    InitEnemyBulletPool();

}

static SceneNode* lastScene = NULL;

int UpdateGame(void) {
    if (player.health <= 0) {
        return 2; 
    }

    SceneNode* currentScene = GetCurrentScene();
    
    if (currentScene != lastScene) 
    {
        IA_IniciaCena(currentScene, &player);
        
        totalEnemiesInScene = IA_GetTotalInimigos(); 
        
        lastScene = currentScene; 
    }
    
    IA_Update(GetFrameTime());

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
    
    return 0; 
}

void DrawEnemyCounter(void) {
    if (totalEnemiesInScene == 0) {
        return;
    }

    int remaining = GetActiveEnemyCount();
    
    if (remaining == 0 && !IA_EstaAtiva()) {
        const char *text = "LIMPO!";
        int fontSize = 30;
        int textWidth = MeasureText(text, fontSize);
        int posX = screenWidth - textWidth - 20;
        int posY = 20;
        DrawText(text, posX, posY, fontSize, GREEN);
    } else {
        const char *text = TextFormat("INIMIGOS: %d / %d", remaining, totalEnemiesInScene);
        int fontSize = 30;
        int textWidth = MeasureText(text, fontSize);
        int posX = screenWidth - textWidth - 20;
        int posY = 20;
        DrawText(text, posX, posY, fontSize, WHITE);
    }
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
    DrawEnemyCounter();
}

void UnloadGame(void) {
    UnloadMap();
    UnloadPlayer(&player);
    UnloadEnemyAssets();
    UnloadBulletAssets();
    UnloadEnemyBulletAssets();
}