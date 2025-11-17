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
#include "mode_select.h" 


static Player player;
static Player player2; 
static bool isPlayer2Active = false; 

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
CharacterType selectedCharacterP2 = CHAR_FINN; 
GameMode selectedGameMode = GAME_MODE_1P;

bool sceneHasCaixa[TOTAL_SCENES + 1];
int extraLives;

#define TOTAL_IMAGENS_CUTSCENE 6
static bool emCutsceneMapa5 = false;
static bool cutsceneMapa5JaExibida = false;
static int indiceImagemCutscene = 0;
static Texture2D imagensCutscene[TOTAL_IMAGENS_CUTSCENE];

static float tempoImagemCutscene = 0.0f;
static const float DURACAO_POR_IMAGEM = 1.0f; 

void InitGame(void) {

    for (int i = 0; i <= TOTAL_SCENES; i++) {
        sceneHasCaixa[i] = false;
    }
    sceneHasCaixa[1] = true;
    sceneHasCaixa[2] = true;
    sceneHasCaixa[3] = true;
    extraLives = 1; 

    emCutsceneMapa5 = false;
    cutsceneMapa5JaExibida = false;
    indiceImagemCutscene = 0;
    tempoImagemCutscene = 0.0f;

    imagensCutscene[0] = LoadTexture("assets/Sprites/cutscene/cutscene1.png");
    imagensCutscene[1] = LoadTexture("assets/Sprites/cutscene/cutscene2.png");
    imagensCutscene[2] = LoadTexture("assets/Sprites/cutscene/cutscene3.png");
    imagensCutscene[3] = LoadTexture("assets/Sprites/cutscene/cutscene4.png");
    imagensCutscene[4] = LoadTexture("assets/Sprites/cutscene/cutscene5.png");
    imagensCutscene[5] = LoadTexture("assets/Sprites/cutscene/cutscene6.png");

    Gemini_Init();
    InitMap();

    srand(time(NULL));
    
    selectedGameMode = GetGameMode();
    isPlayer2Active = (selectedGameMode == GAME_MODE_2P);

    InitPlayer(&player, selectedCharacter, screenWidth / 2 - 50, screenHeight / 2);
    
    if (isPlayer2Active) {
        InitPlayer(&player2, selectedCharacterP2, screenWidth / 2 + 50, screenHeight / 2);
    }

    
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

    if (emCutsceneMapa5) {
        
        tempoImagemCutscene += GetFrameTime();

        bool imagemTerminou = false;
        
        if (indiceImagemCutscene == (TOTAL_IMAGENS_CUTSCENE - 1)) { 
            if (tempoImagemCutscene >= 3.0f) { 
                imagemTerminou = true;
            }
        } else {
            if (tempoImagemCutscene >= DURACAO_POR_IMAGEM) { 
                imagemTerminou = true;
            }
        }

        if (imagemTerminou) {
            tempoImagemCutscene = 0.0f;
            indiceImagemCutscene++;     
            
            if (indiceImagemCutscene >= TOTAL_IMAGENS_CUTSCENE) {
                emCutsceneMapa5 = false;
                
                SceneNode* cenaAtual = GetCurrentScene();
                ControleSpawn_IniciaCena(cenaAtual, &player);
                totalEnemiesInScene = ControleSpawn_GetTotalInimigos();
            }
        }
        
        return 0; 
    }


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


    if (player.health <= 0 && player.isAlive) {
        if (extraLives > 0) {
            extraLives--;
            player.health = player.maxHealth; 
            PlaySound(extraLifeSound);
        } else {
            player.isAlive = false; 
        }
    }
 
    if (isPlayer2Active && player2.health <= 0 && player2.isAlive) {
        if (extraLives > 0) {
            extraLives--;
            player2.health = player2.maxHealth;
            PlaySound(extraLifeSound);
        } else {
            player2.isAlive = false; 
        }
    }

    bool allPlayersDead = (!player.isAlive && (!isPlayer2Active || !player2.isAlive));
    
    if (allPlayersDead) {
        if (!Score_IsPlayerDead()) {
            StopMusicStream(gameMusic);
            PlaySound(gameOverSound); 
            
            Score_SetPlayerDead(true); 
            Score_CalculateFinal();
        }
    }

    
    SceneNode* currentScene = GetCurrentScene();
    
    if (currentScene != lastScene) {
        
        if (currentScene->id == 5 && !cutsceneMapa5JaExibida) {
            emCutsceneMapa5 = true;
            indiceImagemCutscene = 0;
            tempoImagemCutscene = 0.0f; 
            cutsceneMapa5JaExibida = true;
        } else {
            ControleSpawn_IniciaCena(currentScene, &player);
            totalEnemiesInScene = ControleSpawn_GetTotalInimigos();
        }
        
        lastScene = currentScene; 
    }
  
    ControleSpawn_Update(GetFrameTime(), &player);
    Boss_Update(&player); 

    if (Boss_IsDefeated() && GetActiveEnemyCount() == 0) {
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
        bool p1WantsChange = UpdatePlayer(&player, screenWidth, screenHeight, currentScene, false);
        bool p2WantsChange = false;
        
        if (isPlayer2Active) {
            p2WantsChange = UpdatePlayer(&player2, screenWidth, screenHeight, currentScene, true);
        }

        if (p1WantsChange || p2WantsChange) {
            SceneNode* current = GetCurrentScene(); 
            
            DespawnAllEnemies();
            DespawnAllPlayerBullets(); 
            DespawnAllEnemyBullets(); 
            Item_DespawnAll();
            Caixa_DespawnAll();

            current->isCleared = true;
            
            SetCurrentScene(current->next);
            
            player.position.x = 10.0f;
            
            if (isPlayer2Active) {
                player2.position.x = 10.0f;
                player2.position.y = player.position.y; 
            }
        }
    }

    if (player.charType == CHAR_JOHNNY) 
    {
        UpdateBulletPool(screenWidth, screenHeight);
    }
    else if (isPlayer2Active && player2.charType == CHAR_JOHNNY)
    {
        UpdateBulletPool(screenWidth, screenHeight);
    }

    
    UpdateEnemyPool(&player, &player2, isPlayer2Active, screenHeight, currentScene);
    UpdateEnemyBulletPool(screenWidth, screenHeight);

    
    Item_Update(&player);
    if(isPlayer2Active) Item_Update(&player2); 

    Caixa_Update(&player, &player2, isPlayer2Active);

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
        
    } 
}

void DrawGame(void) {
    ClearBackground(DARKGRAY);

    if (emCutsceneMapa5) {
        ClearBackground(BLACK); 
        
        if (indiceImagemCutscene < TOTAL_IMAGENS_CUTSCENE) {
            DrawTexturePro(
                imagensCutscene[indiceImagemCutscene],
                (Rectangle){ 0, 0, (float)imagensCutscene[indiceImagemCutscene].width, (float)imagensCutscene[indiceImagemCutscene].height },
                (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight },
                (Vector2){ 0, 0 },
                0.0f,
                WHITE
            );
            
            if (indiceImagemCutscene == (TOTAL_IMAGENS_CUTSCENE - 1)) {
                const char* texto = "Algo sai da luz, se prepare pra batalha...";
                int fontSize = 30;
                int textWidth = MeasureText(texto, fontSize);
                
                int posX = (screenWidth - textWidth) / 2;
                int posY = screenHeight - 80;
                
                DrawText(texto, posX + 2, posY + 2, fontSize, BLACK);

                DrawText(texto, posX, posY, fontSize, WHITE);
            }
        }
        
        return; 
    }


    DrawCurrentMap();

    Caixa_Draw();
    Item_Draw();      

    DrawPlayer(&player); 
    if (isPlayer2Active) {
        DrawPlayer(&player2); 
    }
    
    DrawEnemyPool();
    DrawEnemyBulletPool();

    if (player.charType == CHAR_JOHNNY)
    {
        DrawAmmoCount(Player_IsReloading(&player), 1);
    }

    if (isPlayer2Active && player2.charType == CHAR_JOHNNY)
    {
        DrawAmmoCount(Player_IsReloading(&player2), 2);
    }

    DrawBulletPool(); 
    
    DrawPlayerHealthBar(&player, false);
    
    if (isPlayer2Active) {
        DrawPlayerHealthBar(&player2, true);
    }

    
    if (!emContagemInicial) {
        DrawEnemyCounter();
    }

    // ##################################################################
    // ## MODIFICAÇÃO INICIA AQUI: HUD de Score e Tempo
    // ##################################################################

    const char* scoreText = TextFormat("SCORE: %08i", Score_GetScore());
    const char* timeText = TextFormat("TEMPO: %04.1f", Score_GetTimer());
    
    int fontSize = 30; // "Maior"
    int padding = 40;
    
    int scoreWidth = MeasureText(scoreText, fontSize);
    int timeWidth = MeasureText(timeText, fontSize);
    int totalWidth = scoreWidth + timeWidth + padding;
    
    int scoreX = (screenWidth - totalWidth) / 2;
    int timeX = scoreX + scoreWidth + padding;
    int textY = 20; // Posição Y no topo
    
    DrawText(scoreText, scoreX, textY, fontSize, YELLOW);
    DrawText(timeText, timeX, textY, fontSize, YELLOW);

    // REMOVIDO: A lógica de desenhar "LIVES" foi movida para DrawPlayerHealthBar em player.c
    // int hudCenterX = screenWidth / 2 - 100;
    // DrawText(TextFormat("SCORE: %08i", Score_GetScore()), hudCenterX, 20, 20, RAYWHITE);
    // DrawText(TextFormat("TIME: %04.1f", Score_GetTimer()), hudCenterX, 45, 20, RAYWHITE);
    // DrawText(TextFormat("LIVES: %d", extraLives), hudCenterX, 70, 20, RAYWHITE); 
    
    // ##################################################################
    // ## MODIFICAÇÃO TERMINA AQUI
    // ##################################################################
    

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

    for (int i = 0; i < TOTAL_IMAGENS_CUTSCENE; i++) {
        UnloadTexture(imagensCutscene[i]);
    }


    UnloadMap();
    UnloadPlayer(&player);
    if (isPlayer2Active) {
        UnloadPlayer(&player2);
    }
    UnloadEnemyAssets();
    UnloadBulletAssets();
    UnloadEnemyBulletAssets();
    Item_Unload();
    Caixa_Unload(); 
}