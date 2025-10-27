#include "sobre.h"

void DrawSobreScreen(void) {
    ClearBackground((Color){20, 20, 30, 255});
    DrawText("SMASH TOONS", 250, 80, 50, YELLOW);
    DrawText("Um jogo Beat 'em Up desenvolvido em C com Raylib.", 120, 200, 20, LIGHTGRAY);
    DrawText("Desenvolvido por Andre Avelino Freitas de Oliveira.", 120, 230, 20, LIGHTGRAY);
    DrawText("Pressione ESC para voltar", 250, 400, 20, GRAY);
}

int UpdateSobreScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) return 1;
    return 0;
}
