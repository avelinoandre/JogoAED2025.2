#include "sobre.h"

void DrawSobreScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){20, 20, 30, 255});

    DrawText("SMASH TOONS", screenWidth/2 - 300, 120, 80, YELLOW);

    DrawText("Um jogo Beat 'em Up desenvolvido em C com Raylib.", screenWidth/2 - 420, 240, 35, LIGHTGRAY);
    DrawText("Desenvolvido por André, Caio, Gabriel e Rodrigo.", screenWidth/2 - 420, 300, 35, LIGHTGRAY);

    DrawText("Pressione ENTER para voltar", screenWidth/2 - 180, screenHeight - 80, 20, GRAY);
}

int UpdateSobreScreen(void) {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE)) {
        return 1;
    }
    return 0;
}