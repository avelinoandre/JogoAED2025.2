#include "comoJogar.h"

void DrawComoJogarScreen(void) {
    ClearBackground((Color){20, 20, 30, 255});
    DrawText("COMO JOGAR", 260, 80, 50, YELLOW);
    DrawText("W, A, S, D - Mover o personagem", 180, 200, 20, LIGHTGRAY);
    DrawText("ESPACO - Atacar", 180, 230, 20, LIGHTGRAY);
    DrawText("ESC - Voltar ao menu", 180, 260, 20, LIGHTGRAY);
    DrawText("Pressione ESC para voltar", 250, 400, 20, GRAY);
}

int UpdateComoJogarScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) return 1;
    return 0;
}
