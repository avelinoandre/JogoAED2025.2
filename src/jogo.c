#include "jogo.h"
#include "player.h"
#include "raylib.h"

static Player player;
static const int screenWidth = 1600;
static const int screenHeight = 900;

void InitGame(void) {
    InitPlayer(&player, screenWidth / 2, screenHeight / 2);
}

int UpdateGame(void) {
    UpdatePlayer(&player, screenWidth, screenHeight);
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        return 1;
    }
    return 0;
}

void DrawGame(void) {
    ClearBackground(DARKGRAY);
    DrawPlayer(&player);
    
    DrawPlayerHealthBar(&player);
    
}

void UnloadGame(void) {
    UnloadPlayer(&player);
}