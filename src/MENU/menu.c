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
    
    menu->background = LoadTexture("assets/Sprites/background/menu/content.png");
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

    // const char *title = "SMASH TOONS";
    // int titleSize = 80;
    // Vector2 titleSizeMeasure = MeasureTextEx(menu->font, title, titleSize, 4);
    

    // DrawTextEx(menu->font, title,
    //     (Vector2){(screenWidth - titleSizeMeasure.x) / 2 - 275, 120 + 4},
    //     titleSize, 4, BLACK);

    // DrawTextEx(menu->font, title,
    //     (Vector2){(screenWidth - titleSizeMeasure.x) / 2 - 271, 120},
    //     titleSize, 4, YELLOW);

    int spacing = 75; 
    int baseY = 250;

    float panelHeight = (float)(MENU_OPTION_COUNT * spacing) + 40.0f;

    Rectangle panel = { 
        (float)screenWidth / 2 - 250, 
        (float)baseY - 20, 
        500, 
        panelHeight 
    };

    DrawRectangleRec(panel, (Color){0, 0, 0, 150});
    DrawRectangleLinesEx(panel, 2, (Color){255, 255, 255, 50});

    for (int i = 0; i < MENU_OPTION_COUNT; i++) {
        
        Color color;
        int fontSize;
        const char* text;

        if (i == menu->selected) {
            float pulse = (sinf(menu->animTimer * 1.8f) + 1.0f) / 2.0f; 
            
            color = ORANGE;
            fontSize = 45 + (int)(pulse * 10.0f);
            text = TextFormat("> %s <", options[i]);
        } else {
            color = RAYWHITE;
            fontSize = 40;
            text = options[i];
        }

        Vector2 textSize = MeasureTextEx(menu->font, text, fontSize, 2);
        DrawTextEx(menu->font, text,
            (Vector2){screenWidth/2 - textSize.x/2 - 100, (float)baseY + i * spacing}, 
            fontSize, 2, color);
    }

    const char* instruction = "Use SETA CIMA e SETA BAIXO para navegar, ENTER para selecionar";
    Vector2 instructionSize = MeasureTextEx(GetFontDefault(), instruction, 20, 1);
    DrawText(instruction,
             (screenWidth - (int)instructionSize.x) / 2, screenHeight - 60, 20, LIGHTGRAY);
}

void UnloadMenu(Menu *menu) {
    UnloadFont(menu->font);
    UnloadTexture(menu->background);
}