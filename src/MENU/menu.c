#include "menu.h"

static const char *options[MENU_OPTION_COUNT] = {
    "JOGAR",
    "SOBRE",
    "COMO JOGAR",
    "SAIR"
};

void InitMenu(Menu *menu) {
    menu->selected = MENU_OPTION_PLAY;
    menu->font = LoadFont("assets/fonts/pixelfont.ttf");
}

MenuOption UpdateMenu(Menu *menu) {
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

    ClearBackground((Color){30, 30, 40, 255});

    const char *title = "SMASH TOONS";
    int titleSize = 80;
    Vector2 titleSizeMeasure = MeasureTextEx(menu->font, title, titleSize, 4);
    DrawTextEx(menu->font, title,
        (Vector2){screenWidth/3 - titleSizeMeasure.x/2, 120},
        titleSize, 4, YELLOW);

    int baseY = 200;
    int spacing = 70;

    for (int i = 0; i < MENU_OPTION_COUNT; i++) {
        
        Color color;
        int fontSize;

        if (i == menu->selected) {
            color = ORANGE;
            fontSize = 50;
        } else {
            color = RAYWHITE;
            fontSize = 40;
        }

        Vector2 textSize = MeasureTextEx(menu->font, options[i], fontSize, 2);
        DrawTextEx(menu->font, options[i],
            (Vector2){screenWidth/2 - 60 - textSize.x/2, (baseY + 100) + i * spacing},
            fontSize, 2, color);
    }

    DrawText("Use SETA CIMA e SETA BAIXO para navegar, ENTER para selecionar",
             screenWidth/3 - 70, screenHeight - 80, 20, GRAY);
}

void UnloadMenu(Menu *menu) {
    UnloadFont(menu->font);
}