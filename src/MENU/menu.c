#include "menu.h"
#include <math.h>

static const char *options[MENU_OPTION_COUNT] = {
    "JOGAR",
    "RANKING",
    "SOBRE",
    "COMO JOGAR",
    "SAIR"
};

void InitMenu(Menu *menu) {
    menu->selected = MENU_OPTION_PLAY;
    menu->font = LoadFont("assets/fonts/pixelfont.ttf");
    
    menu->background = LoadTexture("assets/Sprites/background/menu/BACKGROUND.png");
    menu->animTimer = 0.0f;
}

MenuOption UpdateMenu(Menu *menu) {
    menu->animTimer += GetFrameTime();

    if (IsKeyPressed(KEY_DOWN)) {
        menu->selected++;
        if (menu->selected >= MENU_OPTION_COUNT)
            menu->selected = 0;
    }
    if (IsKeyPressed(KEY_UP)) {
        if (menu->selected == 0)
            menu->selected = MENU_OPTION_COUNT - 1;
        else
            menu->selected--;
    }
    if (IsKeyPressed(KEY_ENTER))
        return menu->selected;
    return -1;
}

void DrawMenu(Menu *menu) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    DrawTexturePro(
        menu->background,
        (Rectangle){ 0, 0, (float)menu->background.width, (float)menu->background.height },
        (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    int spacing = 75;
    
    const int baseFontSize = 40;
    const int selectedFontSize = 50;

    int totalMenuHeight = (MENU_OPTION_COUNT - 1) * spacing;
    int baseY = (screenHeight - totalMenuHeight) / 2;

    for (int i = 0; i < MENU_OPTION_COUNT; i++) {
        
        Color color;
        int fontSize;
        const char* text;

        if (i == menu->selected) {
            color = YELLOW;
            fontSize = selectedFontSize;
            text = TextFormat("> %s", options[i]);
        } else {
            color = RAYWHITE;
            fontSize = baseFontSize;
            text = options[i];
        }

        Vector2 textSize = MeasureTextEx(menu->font, text, fontSize, 2);
        
        float optionX = (screenWidth - textSize.x) / 2.0f - 70; 
        float optionY = (float)baseY + i * spacing;
        
        if (i == menu->selected) {
            optionY -= (selectedFontSize - baseFontSize) / 2.0f;
        }

        float shadowOffset = 3.0f;

        DrawTextEx(menu->font, text,
            (Vector2){optionX + shadowOffset, optionY + 80 + shadowOffset}, 
            fontSize, 2, BLACK);

        DrawTextEx(menu->font, text,
            (Vector2){optionX, optionY + 80}, 
            fontSize, 2, color);
    }

    const char* instruction = "Use SETA CIMA e SETA BAIXO para navegar, ENTER para selecionar";
    Vector2 instructionSize = MeasureTextEx(GetFontDefault(), instruction, 20, 1);
    
    DrawText(instruction,
             (screenWidth - (int)instructionSize.x) / 2,
             screenHeight - 60, 20, LIGHTGRAY);
}

void UnloadMenu(Menu *menu) {
    UnloadFont(menu->font);
    UnloadTexture(menu->background);
}