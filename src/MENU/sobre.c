#include "sobre.h"

void DrawSobreScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){20, 20, 30, 255});

    DrawText("SMASH TOONS", screenWidth/2 - 300, 120, 80, YELLOW);

    DrawText("O macaco louco, junto com outros vilões, criaram um portal", screenWidth/2 - 500, 240, 35, LIGHTGRAY);
    DrawText("para viajar pelo multiverso e roubar todos os poderes", screenWidth/2 - 500, 300, 35, LIGHTGRAY);
    DrawText("dos heróis da Cartoon, os  quais se uniram para enfrenta-los", screenWidth/2 - 500, 360, 35, LIGHTGRAY);
    DrawText("e defender o multiverso da Cartoon dos vilões", screenWidth/2 - 500, 400, 35, LIGHTGRAY);

    DrawText("Pressione BACKSPACE para voltar", screenWidth/2 - 180, screenHeight - 80, 20, GRAY);
}

int UpdateSobreScreen(void) {
    if (IsKeyPressed(KEY_BACKSPACE)) {
        return 1;
    }
    return 0;
}