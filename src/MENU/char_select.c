#include "char_select.h"
#include "menu.h" 
#include "mode_select.h" 
#include <stdio.h> 

#define MAIN_STATE_MENU 0
#define MAIN_STATE_MODE_SELECT 1
#define MAIN_STATE_CHAR_SELECT 2
#define MAIN_STATE_GAME 3


static const char *charNames[] = {
    "JOHNNY BRAVO",
    "FINN",
    "SAMURAI JACK",
    "MORDECAI"
};

static const char *statsNames[] = {"VIDA", "PODER","VEL"};
static const char *statsGrades[4][3] = {
    {"A", "B","C"}, 
    {"B", "B","A"},
    {"A", "A","B"}, 
    {"A", "A","A"}  
};


static int selectedOption = 0;
static Font font;
static Texture2D portraits[4];
static Texture2D portraits_icon[4];

static bool isSelectingP2 = false; 

void InitCharSelectMenu(void) {
    selectedOption = 0;
    isSelectingP2 = false; 

    font = LoadFont("assets/fonts/pixelfont.ttf"); 

    portraits[0] = LoadTexture("assets/Sprites/JohnyBravo/ataque/JohnyBravo21.png");
    portraits[1] = LoadTexture("assets/Sprites/Finn/Finnataque/finn26.png");
    portraits[2] = LoadTexture("assets/Sprites/Samurai/Samuraiataque/Samurai26.png");
    portraits[3] = LoadTexture("assets/Sprites/Mordecai/Mordecaidescoladocomemoracao/mordecai77.png");

    portraits_icon[0] = LoadTexture("assets/Sprites/icons/johnyBravo_icon.png");
    portraits_icon[1] = LoadTexture("assets/Sprites/icons/finn_icon.png"); 
    portraits_icon[2] = LoadTexture("assets/Sprites/icons/samuraiJack_icon.png");
    portraits_icon[3] = LoadTexture("assets/Sprites/icons/mordecai_icon.png");

}

void UpdateCharSelectMenu(int *gameState) {
    if (IsKeyPressed(KEY_RIGHT)) {
        selectedOption++;
        if (selectedOption >= 4) selectedOption = 0;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = 3;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (!isSelectingP2) {
            // Selecionando P1
            selectedCharacter = (CharacterType)selectedOption;
            
            if (GetGameMode() == GAME_MODE_2P) {
                // Se for 2P, prepara para selecionar P2
                isSelectingP2 = true;
                selectedOption = 1; // Padrão P2 para Finn
            } else {
                // Se for 1P, vai direto para o jogo
                *gameState = MAIN_STATE_GAME; // CORREÇÃO: Usa o valor de main.c
            }
        } else {
            // Selecionando P2
            selectedCharacterP2 = (CharacterType)selectedOption;
            isSelectingP2 = false; // Reseta
            *gameState = MAIN_STATE_GAME; // CORREÇÃO: Usa o valor de main.c
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (isSelectingP2) {
            isSelectingP2 = false; 
        } else {
            *gameState = MAIN_STATE_MODE_SELECT; 
        }
    }
}

void DrawCharSelectMenu(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    ClearBackground((Color){30, 30, 40, 255});

    const char *title = isSelectingP2 ? "ESCOLHA SEU LUTADOR (P2)" : "ESCOLHA SEU LUTADOR (P1)";
    Vector2 titleSize = MeasureTextEx(font, title, 60, 4);
    DrawTextEx(font, title, (Vector2){(screenWidth - titleSize.x) / 3.5, 60}, 60, 4, YELLOW);

    int charCount = 4;
    int columnWidth = 280;
    int columnHeight = 550;
    int spacing = 40;
    int totalWidth = (columnWidth * charCount) + (spacing * (charCount - 1));
    int startX = (screenWidth - totalWidth) / 2;
    int startY = (screenHeight - columnHeight) / 2 + 30; 

    static float nameOffsetsX[4] = {
        30.0f, 
        105.0f,
        30.0f,  
        70.0f   
    };

    for (int i = 0; i < charCount; i++) {
        
        int currentX = startX + i * (columnWidth + spacing);
        Rectangle columnRect = { (float)currentX, (float)startY, (float)columnWidth, (float)columnHeight };

        Color boxColor = (i == selectedOption) ? ORANGE : DARKGRAY;
        DrawRectangleRec(columnRect, (Color){10, 10, 15, 200}); 
        DrawRectangleLinesEx(columnRect, 4, boxColor);

        if (isSelectingP2 && i == (int)selectedCharacter) {
             DrawRectangle(currentX, startY - 30, columnWidth, 30, BLUE);
             Vector2 tagSize = MeasureTextEx(font, "1 PLAYER", 20, 2);
             DrawTextEx(font, "1 PLAYER", (Vector2){currentX + (columnWidth - tagSize.x)/2, startY - 25}, 20, 2, WHITE);
        }

        if (i == selectedOption) {
            const char* playerTag = isSelectingP2 ? "2 PLAYER" : "1 PLAYER";
            DrawRectangle(currentX, startY - 30, columnWidth, 30, ORANGE);
            Vector2 tagSize = MeasureTextEx(font, playerTag, 20, 2);
            DrawTextEx(font, playerTag, (Vector2){currentX + (columnWidth - tagSize.x)/2, startY - 25}, 20, 2, BLACK);
        }

        if (portraits[i].id > 0) {
            float portraitHeight = 200;
            float scale = portraitHeight / portraits[i].height;
            float portraitWidth = portraits[i].width * scale;
            Rectangle srcRect = { 0, 0, (float)portraits[i].width, (float)portraits[i].height };
            Rectangle destRect = {
                currentX + (columnWidth - portraitWidth) / 2, 
                startY + 40,
                portraitWidth,
                portraitHeight
            };
            DrawTexturePro(portraits[i], srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        }

        int nameY = startY + 260;
        
        float posX = (float)currentX + nameOffsetsX[i]; 
            
        DrawTextEx(font, charNames[i], (Vector2){posX, (float)nameY}, 30, 2, RAYWHITE);

        int statsY = nameY + 60;
        int statsPadding = 35;
        for (int j = 0; j < 3; j++) {
            DrawTextEx(font, statsNames[j], (Vector2){(float)currentX + 30, (float)statsY + j * statsPadding}, 30, 2, LIGHTGRAY);
            DrawTextEx(font, statsGrades[i][j], (Vector2){(float)currentX + columnWidth - 60, (float)statsY + j * statsPadding}, 30, 2, RAYWHITE);
        }

        if (portraits_icon[i].id > 0) {
            float smallSpriteHeight = 100;
            float scale = smallSpriteHeight / portraits_icon[i].height;
            float smallSpriteWidth = portraits_icon[i].width * scale;
            Rectangle srcRect = { 0, 0, (float)portraits_icon[i].width, (float)portraits_icon[i].height };
            Rectangle destRect = {
                currentX + (columnWidth - smallSpriteWidth) / 2, 
                statsY + 140,
                smallSpriteWidth,
                smallSpriteHeight
            };
            DrawTexturePro(portraits_icon[i], srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}


void UnloadCharSelectMenu(void) {
    UnloadFont(font);
    
    for (int i = 0; i < 4; i++) {
        if (portraits[i].id > 0) {
            UnloadTexture(portraits[i]);
        }
    }

    for (int i = 0; i < 4; i++) {
        if (portraits_icon[i].id > 0) {
            UnloadTexture(portraits_icon[i]);
        }
    }
}