#include "sobre.h"

void DrawSobreScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){20, 20, 30, 255});

    DrawText("SMASH TOONS", screenWidth/2 - 250, 120, 80, YELLOW);

    DrawText("Um jogo Beat 'em Up desenvolvido em C com Raylib.", screenWidth/2 - 290, 240, 35, LIGHTGRAY);
    DrawText("Desenvolvido por André, Caio, Gabriel e Rodrigo.", screenWidth/2 - 290, 300, 35, LIGHTGRAY);

    DrawText("Pressione ESC para voltar", screenWidth/2 - 120, screenHeight - 80, 20, GRAY);
}

int UpdateSobreScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) return 1;
    return 0;
}