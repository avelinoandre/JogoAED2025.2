#include "mode_select.h"
#include <math.h>

static const char *options[] = {
    "1 JOGADOR",
    "2 JOGADORES"
};
#define MODE_OPTION_COUNT 2

static int selectedOption = 0;
static Font font;
static float animTimer = 0.0f;

void InitModeSelect(void) {
    selectedOption = 0;
    font = LoadFont("assets/fonts/pixelfont.ttf");
    if (font.texture.id == 0) {
        font = GetFontDefault();
    }
}

int UpdateModeSelect(void) {
    animTimer += GetFrameTime();

    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
        if (selectedOption >= MODE_OPTION_COUNT) selectedOption = 0;
    }
    if (IsKeyPressed(KEY_UP)) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = MODE_OPTION_COUNT - 1;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        selectedGameMode = (selectedOption == 0) ? GAME_MODE_1P : GAME_MODE_2P;
        return selectedOption + 1;
    }

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
        return 0; 
    }
    
    return -1; 
}

void DrawModeSelect(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){30, 30, 40, 255});

    const char *title = "SELECIONE O MODO";
    Vector2 titleSize = MeasureTextEx(font, title, 70, 4);
    DrawTextEx(font, title, (Vector2){(screenWidth - titleSize.x) / 2, 150}, 70, 4, YELLOW);

    int spacing = 90; 
    int baseY = 350;

    for (int i = 0; i < MODE_OPTION_COUNT; i++) {
        Color color;
        int fontSize;
        const char* text;

        if (i == selectedOption) {
            float pulse = (sinf(animTimer * 1.8f) + 1.0f) / 2.0f; 
            color = ORANGE;
            fontSize = 55 + (int)(pulse * 10.0f);
            text = TextFormat("> %s <", options[i]);
        } else {
            color = RAYWHITE;
            fontSize = 50;
            text = options[i];
        }

        Vector2 textSize = MeasureTextEx(font, text, fontSize, 2);
        DrawTextEx(font, text,
            (Vector2){(screenWidth - textSize.x) / 2, (float)baseY + i * spacing}, 
            fontSize, 2, color);
    }

    const char* instruction = "Use SETAS para navegar, ENTER para selecionar, BACKSPACE para voltar";
    Vector2 instructionSize = MeasureTextEx(GetFontDefault(), instruction, 20, 1);
    DrawText(instruction,
             (screenWidth - (int)instructionSize.x) / 2, screenHeight - 60, 20, LIGHTGRAY);
}

void UnloadModeSelect(void) {
    if (font.texture.id != GetFontDefault().texture.id) {
        UnloadFont(font);
    }
}

GameMode GetGameMode(void) {
    return selectedGameMode;
}