#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum {
    MENU_OPTION_PLAY,
    MENU_OPTION_ABOUT,
    MENU_OPTION_HOW_TO_PLAY,
    MENU_OPTION_EXIT,
    MENU_OPTION_COUNT
} MenuOption;

typedef struct {
    MenuOption selected;
    Font font;
} Menu;

void InitMenu(Menu *menu);
MenuOption UpdateMenu(Menu *menu);
void DrawMenu(Menu *menu);
void UnloadMenu(Menu *menu);

#endif