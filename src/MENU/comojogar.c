#include "comoJogar.h"

void DrawComoJogarScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    ClearBackground((Color){20, 20, 30, 255});

    DrawText("COMO JOGAR", screenWidth/2 - 280, 80, 80, YELLOW);

    DrawText("PLAYER 1:", screenWidth/2 - 300, 180, 35, LIGHTGRAY);
    DrawText("W, A, S, D - Mover o personagem", screenWidth/2 - 300, 240, 35, LIGHTGRAY);
    DrawText("SPACE - Atacar", screenWidth/2 - 300, 300, 35, LIGHTGRAY);
    DrawText("R - Recarregar", screenWidth/2 - 300, 360, 35, LIGHTGRAY);

    DrawText("PLAYER 2:", screenWidth/2 - 300, 420, 35, LIGHTGRAY);
    DrawText("^, <, > , v - Mover o personagem", screenWidth/2 - 300, 480, 35, LIGHTGRAY);
    DrawText("L - Atacar", screenWidth/2 - 300, 540, 35, LIGHTGRAY);
    DrawText("R - Recarregar", screenWidth/2 - 300, 600, 35, LIGHTGRAY);
    

    DrawText("ENTER - Voltar ao menu", screenWidth/2 - 300, 660, 35, LIGHTGRAY);

     DrawText("Pressione BACKSPACE para voltar", screenWidth/2 - 180, screenHeight - 80, 20, GRAY);
}

int UpdateComoJogarScreen(void) {
    if (IsKeyPressed(KEY_BACKSPACE)){
        return 1;
    }
    return 0;
}