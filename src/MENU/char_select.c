#include "char_select.h"
#include "menu.h"
#include <stdio.h> 


typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_SOBRE,
    STATE_COMO_JOGAR,
    STATE_CHAR_SELECT,
    STATE_EXIT
} GameState;


static const char *charNames[] = {
    "JOHNNY BRAVO",
    "FINN",
    "SAMURAI JACK",
    "MORDECAI"
};

static int selectedOption = 0;
static Font font;
static Texture2D portraits[4]; 

void InitCharSelectMenu(void) {
    selectedOption = 0;
    font = LoadFont("assets/fonts/pixelfont.ttf"); 

    portraits[0] = LoadTexture("assets/Sprites/JohnyBravo/parado/JohnyBravo1.png");
    portraits[1] = LoadTexture("assets/Sprites/Finn/Finnparado/finn1.png");
    portraits[2] = LoadTexture("assets/Sprites/Samurai/Samuraiparado/Samurai1.png");
    portraits[3] = LoadTexture("assets/Sprites/Mordecai/Mordecaiparado/mordecai8.png");
}

void UpdateCharSelectMenu(int *gameState) {
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
        if (selectedOption >= 4) selectedOption = 0;
    }
    if (IsKeyPressed(KEY_UP)) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = 3;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        selectedCharacter = (CharacterType)selectedOption;
        *gameState = STATE_GAME; 
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        *gameState = STATE_MENU;
    }
}

void DrawCharSelectMenu(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){30, 30, 40, 255});

    const char *title = "ESCOLHA SEU LUTADOR";
    Vector2 titleSize = MeasureTextEx(font, title, 60, 4);
    DrawTextEx(font, title, (Vector2){(screenWidth - titleSize.x) / 4, 80}, 60, 4, YELLOW);

    int baseY = 200;
    int spacing = 100;
    int portraitSize = 150;

    for (int i = 0; i < 4; i++) {
        Color color = (i == selectedOption) ? ORANGE : RAYWHITE;
        int fontSize = (i == selectedOption) ? 40 : 30;

        Vector2 textSize = MeasureTextEx(font, charNames[i], fontSize, 2);

        if (portraits[i].id > 0) {
            Rectangle srcRect = { 0, 0, (float)portraits[i].width, (float)portraits[i].height };
            Rectangle destRect = { 
                (float)(screenWidth / 2) - 150 - (portraitSize / 2), 
                (float)baseY + (i * spacing), 
                (float)portraitSize, 
                (float)portraitSize 
            };
            DrawTexturePro(portraits[i], srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        }

        DrawTextEx(font, charNames[i],
            (Vector2){(float)(screenWidth / 2) - textSize.x / 2 + 100, (float)baseY + (i * spacing) + (portraitSize / 2) - (textSize.y / 2)},
            fontSize, 2, color);
    }
}

void UnloadCharSelectMenu(void) {
    UnloadFont(font);
    for (int i = 0; i < 4; i++) {
        UnloadTexture(portraits[i]);
    }
}