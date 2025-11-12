#include "gemini_ai.h"
#include "jogo.h"
#include "player.h" 
#include "raylib.h"
#include "bullet.h"
#include "enemy.h"
#include "mapa.h"
#include <stdlib.h> 
#include <time.h>
#include "globals.h"
#include "algoritmo_inimigos.h"
#include <stdio.h> 
#include <math.h>
#include "score.h"

static Player player;
static const int screenWidth = 1600;
static const int screenHeight = 900;
static int totalEnemiesInScene = 0;
static SceneNode* lastScene = NULL; 

static bool emContagemInicial;
static float tempoContagem;

CharacterType selectedCharacter = CHAR_JOHNNY;

void InitGame(void) {
    Gemini_Init();
    InitMap();

    srand(time(NULL));
    InitPlayer(&player, screenWidth / 2, screenHeight / 2);
    
    InitEnemyPool();
    InitBulletPool();
    InitEnemyBulletPool();

    lastScene = NULL;

    Score_Init();

    emContagemInicial = true;
    tempoContagem = 0.0f;   
}

int UpdateGame(void) {

    if (emContagemInicial) {
        tempoContagem += GetFrameTime();
        
        if (tempoContagem > 4.0f) {
            emContagemInicial = false; 
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            Score_Init();
            return 1; 
        }

        return 0; 
    }

    if (Score_IsPlayerDead()) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
            Score_Init(); 
            return 1; 
        }
        return 0; 
    }

    Score_Update(GetFrameTime());

    if (player.health <= 0) {
        Score_SetPlayerDead(true);
        Score_CalculateFinal();

        return 2; 
    }
    
    SceneNode* currentScene = GetCurrentScene();
    
    if (currentScene != lastScene) 
    {
        if (currentScene->id == 5) {
            totalEnemiesInScene = 0; 
            DespawnAllEnemies(); 
        } else {
            IA_IniciaCena(currentScene, &player);
            totalEnemiesInScene = IA_GetTotalInimigos();
        }
        
        lastScene = currentScene;
    }
    
    if (currentScene->id != 5) {
         IA_Update(GetFrameTime(), &player);
    }
    

    if (IsKeyPressed(KEY_ESCAPE)) {
        Score_Init();
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
    if (totalEnemiesInScene == 0 && !IA_EstaAtiva()) {
        return;
    }

    int remaining = GetActiveEnemyCount();
    int spawned = IA_GetEnemiesSpawned();
    
    int killed = spawned - remaining;
    
    if (killed < 0) killed = 0; 
    
    if (remaining == 0 && !IA_EstaAtiva()) {
        const char *text = "LIMPO!";
        int fontSize = 30;
        int textWidth = MeasureText(text, fontSize);
        int posX = screenWidth - textWidth - 20;
        int posY = 20;
        DrawText(text, posX, posY, fontSize, GREEN);
    } else {
        const char *text = TextFormat("ELIMINADOS: %d / %d", killed, totalEnemiesInScene);
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

    DrawText(TextFormat("SCORE: %08i", Score_GetScore()), 20, 70, 20, RAYWHITE);
    DrawText(TextFormat("TIME: %04.1f", Score_GetTimer()), 20, 95, 20, RAYWHITE);

    if (Score_IsPlayerDead()) {
        int posX = screenWidth - 320; 
        int posY = 130;

        DrawText("GAME OVER", posX, posY, 40, RED);
        
        const char *scoreText = TextFormat("FINAL SCORE: %08i", Score_GetFinalScore());
        DrawText(scoreText, posX, posY + 50, 20, YELLOW);

        const char *rankText = Score_GetRank();
        DrawText(rankText, posX, posY + 80, 20, WHITE);

        DrawText("Pressione ENTER para voltar", posX - 20, posY + 120, 20, GRAY);
    }

    if (emContagemInicial) {
        const char* textoContagem = "";
        
        if (tempoContagem <= 1.0f) {
            textoContagem = "1";
        } else if (tempoContagem <= 2.0f) {
            textoContagem = "2";
        } else if (tempoContagem <= 3.0f) {
            textoContagem = "3";
        } else if (tempoContagem <= 4.0f) {
            textoContagem = "GO!";
        }

        if (tempoContagem <= 4.0f) {
            int fontSize = 120; 
            int textWidth = MeasureText(textoContagem, fontSize);
            
            int posX = (screenWidth - textWidth) / 2;
            int posY = (screenHeight - fontSize) / 2;
            
            DrawText(textoContagem, posX + 4, posY + 4, fontSize, BLACK);
            DrawText(textoContagem, posX, posY, fontSize, YELLOW);
        }
    }
}

void UnloadGame(void) {
    UnloadMap();
    UnloadPlayer(&player);
    UnloadEnemyAssets();
    UnloadBulletAssets();
    UnloadEnemyBulletAssets();
}