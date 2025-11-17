#include "name_input.h"
#include "raylib.h"
#include "globals.h"
#include <string.h>

static Font font;
static int letterIndex = 0;
char currentPlayerName[4] = "AAA"; 

void InitNameInput(void) {
    font = LoadFont("assets/fonts/pixelfont.ttf");
    if (font.texture.id == 0) font = GetFontDefault();
    
    strcpy(currentPlayerName, "AAA");
    letterIndex = 0;
}

int UpdateNameInput(void) {
    if (IsKeyPressed(KEY_UP)) {
        currentPlayerName[letterIndex]++;
        if (currentPlayerName[letterIndex] > 'Z') currentPlayerName[letterIndex] = 'A';
    }
    if (IsKeyPressed(KEY_DOWN)) {
        currentPlayerName[letterIndex]--;
        if (currentPlayerName[letterIndex] < 'A') currentPlayerName[letterIndex] = 'Z';
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        letterIndex++;
        if (letterIndex > 2) letterIndex = 0;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        letterIndex--;
        if (letterIndex < 0) letterIndex = 2;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        return 1;
    }

    return 0;
}

void DrawNameInput(void) {
    ClearBackground((Color){30, 30, 40, 255});
    const int screenWidth = GetScreenWidth();

    DrawTextEx(font, "DIGITE SEU NOME", (Vector2){screenWidth/2 - 200, 200}, 60, 4, YELLOW);

    int startX = screenWidth / 2 - 100;
    int startY = 400;

    for (int i = 0; i < 3; i++) {
        Color color = (i == letterIndex) ? ORANGE : RAYWHITE;
        
        char letraStr[2] = { currentPlayerName[i], '\0' };
        DrawTextEx(font, letraStr, (Vector2){(float)startX + (i * 80), (float)startY}, 80, 4, color);

        if (i == letterIndex) {
            DrawRectangle(startX + (i * 80), startY + 90, 50, 5, ORANGE);
        }
    }

    DrawText("SETAS para mudar | ENTER para confirmar", screenWidth/2 - 250, 600, 20, LIGHTGRAY);
}

void UnloadNameInput(void) {
    if (font.texture.id != GetFontDefault().texture.id) UnloadFont(font);
}