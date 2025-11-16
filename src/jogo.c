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
#include "boss.h"
#include "caixa.h"
#include "item.h" 


static Player player;
static const int screenWidth = 1600;
static const int screenHeight = 900;
static int totalEnemiesInScene = 0;
static SceneNode* lastScene = NULL; 

static bool emContagemInicial;
static float tempoContagem;

static Music gameMusic;
static Sound gameOverSound;
static Sound extraLifeSound;

CharacterType selectedCharacter = CHAR_JOHNNY;
bool sceneHasCaixa[TOTAL_SCENES + 1];
int extraLives;

void InitGame(void) {

    for (int i = 0; i <= TOTAL_SCENES; i++) {
        sceneHasCaixa[i] = false;
    }
    sceneHasCaixa[1] = true;
    sceneHasCaixa[2] = true;
    sceneHasCaixa[3] = true;
    extraLives = 1; 

    Gemini_Init();
    InitMap();

    srand(time(NULL));
    InitPlayer(&player, screenWidth / 2, screenHeight / 2);
    
    InitEnemyPool();
    InitBulletPool();
    InitEnemyBulletPool();
    Boss_Init();
    Item_Init(); 
    Caixa_Init();  
    
    lastScene = NULL;

    Score_Init();

    emContagemInicial = true;
    tempoContagem = 0.0f;   

    gameMusic = LoadMusicStream("assets/audios/gameAudio.mp3"); 
    SetMusicVolume(gameMusic, 1.0f);
    PlayMusicStream(gameMusic);

    gameOverSound = LoadSound("assets/audios/gameOverSound.wav"); 
    SetSoundVolume(gameOverSound, 0.8f);

    extraLifeSound = LoadSound("assets/audios/vidaExtra.wav");
    SetSoundVolume(extraLifeSound, 0.6f);
}

int UpdateGame(void) {

    UpdateMusicStream(gameMusic);

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
        if (extraLives > 0) {
            extraLives--;
            player.health = player.maxHealth; 
            PlaySound(extraLifeSound);
        } else {
            if (!Score_IsPlayerDead()) {
                StopMusicStream(gameMusic);
                PlaySound(gameOverSound); 
                
                Score_SetPlayerDead(true); 
                Score_CalculateFinal();
            }
        }
    }
    
    SceneNode* currentScene = GetCurrentScene();
    
    if (currentScene != lastScene) {
        ControleSpawn_IniciaCena(currentScene, &player);
        totalEnemiesInScene = ControleSpawn_GetTotalInimigos();
        
        lastScene = currentScene; 
    }
    
    ControleSpawn_Update(GetFrameTime(), &player);
    Boss_Update(&player);

    if (Boss_IsDefeated()) {
        if (!Score_IsPlayerDead()) {
            Score_CalculateFinal(); 
            StopMusicStream(gameMusic);
            
            return 2; 
        }
    }
    

    if (IsKeyPressed(KEY_ESCAPE)) {
        Score_Init();
        return 1; 
    }

    if (!Score_IsPlayerDead()) {
        UpdatePlayer(&player, screenWidth, screenHeight,currentScene);
    }

    if (selectedCharacter == CHAR_JOHNNY) 
    {
        UpdateBulletPool(screenWidth, screenHeight);
    }

    UpdateEnemyPool(&player, screenHeight,currentScene);
    UpdateEnemyBulletPool(screenWidth, screenHeight);
    
    Item_Update(&player);
    Caixa_Update(&player);

    return 0; 
}

void DrawEnemyCounter(void) {
    if (totalEnemiesInScene == 0 && !ControleSpawn_EstaAtivo()) {
        return;
    }

    int remaining = GetActiveEnemyCount();
    int spawned = ControleSpawn_GetInimigosSpawnados();
    
    int killed = spawned - remaining;
    
    if (killed < 0) killed = 0; 
    
    if (remaining == 0 && !ControleSpawn_EstaAtivo()) {
        float pulse = (sin(GetTime() * 8.0f) + 1.0f) / 2.0f;
        int bounceOffset = (int)(pulse * 15.0f);

        const char *text = "GO";
        
        int fontSize = 55;
        float arrowSize = 28.0f; 

        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 0);

        int basePosX = screenWidth - 140; 
        int posY = screenHeight / 2; 

        int animatedPosX = basePosX + bounceOffset;

        int ajusteVerticalSeta = 4;

        DrawText(text, animatedPosX, posY - (int)(textSize.y / 2), fontSize, YELLOW);

        Vector2 p1 = { 
            animatedPosX + textSize.x + 15,
            (float)posY - arrowSize - ajusteVerticalSeta 
        };
        Vector2 p2 = { 
            animatedPosX + textSize.x + 15, 
            (float)posY + arrowSize - ajusteVerticalSeta 
        };
        Vector2 p3 = { 
            animatedPosX + textSize.x + 15 + arrowSize, 
            (float)posY - ajusteVerticalSeta 
        };
        
        DrawTriangle(p1, p2, p3, YELLOW);
        
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

    Caixa_Draw();
    Item_Draw();      

    DrawPlayer(&player);
    
    DrawEnemyPool();
    DrawEnemyBulletPool();

    if (selectedCharacter == CHAR_JOHNNY)
    {
        DrawBulletPool(); 
        DrawAmmoCount(Player_IsReloading(&player));
    }

    DrawPlayerHealthBar(&player);
    DrawEnemyCounter();

    DrawText(TextFormat("SCORE: %08i", Score_GetScore()), 20, 70, 20, RAYWHITE);
    DrawText(TextFormat("TIME: %04.1f", Score_GetTimer()), 20, 95, 20, RAYWHITE);
    DrawText(TextFormat("LIVES: %d", extraLives), 20, 120, 20, RAYWHITE); 
    
    if (Score_IsPlayerDead()) {
        int posX = screenWidth - 923; 
        int posY = 350;

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
            textoContagem = "3";
        } else if (tempoContagem <= 2.0f) {
            textoContagem = "2";
        } else if (tempoContagem <= 3.0f) {
            textoContagem = "1";
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
    StopMusicStream(gameMusic);
    UnloadMusicStream(gameMusic);
    UnloadSound(gameOverSound);
    UnloadSound(extraLifeSound);
    UnloadMap();
    UnloadPlayer(&player);
    UnloadEnemyAssets();
    UnloadBulletAssets();
    UnloadEnemyBulletAssets();
    Item_Unload();
    Caixa_Unload(); 
}