#include "sobre.h"

void DrawSobreScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){20, 20, 30, 255});

    DrawText("SMASH TOONS", 150, 120, 50, YELLOW);

    DrawText("Um jogo Beat 'em Up desenvolvido em C com Raylib.", 120, 240, 20, LIGHTGRAY);
    DrawText("Desenvolvido por Andre Avelino Freitas de Oliveira.", 120, 270, 20, LIGHTGRAY);

    DrawText("Pressione ESC para voltar", 196, screenHeight - 80, 20, GRAY);
}

int UpdateSobreScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) return 1;
    return 0;
}