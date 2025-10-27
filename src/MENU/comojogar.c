#include "comoJogar.h"

void DrawComoJogarScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    ClearBackground((Color){20, 20, 30, 255});

    DrawText("COMO JOGAR", screenWidth/2 - 250, 120, 80, YELLOW);

    DrawText("W, A, S, D - Mover o personagem", screenWidth/2 - 230, 240, 35, LIGHTGRAY);
    DrawText("ESPACO - Atacar", screenWidth/2 - 230, 300, 35, LIGHTGRAY);
    DrawText("ESC - Voltar ao menu", screenWidth/2 - 230, 360, 35, LIGHTGRAY);

    DrawText("Pressione ESC para voltar",screenWidth/2 - 120, screenHeight - 80, 20, GRAY);
}

int UpdateComoJogarScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) return 1;
    return 0;
}