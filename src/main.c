#include "raylib.h"
#include "menu.h"
#include "sobre.h"
#include "comoJogar.h"
#include "jogo.h" 
#include "bullet.h" 

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_SOBRE,
    STATE_COMO_JOGAR,
    STATE_EXIT
} GameState;

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "SMASH TOONS");
    SetTargetFPS(60);

    Menu menu;
    InitMenu(&menu);
    InitGame();
    InitBulletPool();
    GameState state = STATE_MENU;
    bool running = true;

    while (!WindowShouldClose() && running) {
        
        switch (state) {
            case STATE_MENU: {
                int selected = UpdateMenu(&menu);
                if (selected == MENU_OPTION_PLAY) state = STATE_GAME;
                else if (selected == MENU_OPTION_ABOUT) state = STATE_SOBRE;
                else if (selected == MENU_OPTION_HOW_TO_PLAY) state = STATE_COMO_JOGAR;
                else if (selected == MENU_OPTION_EXIT) running = false;
            } break;

            case STATE_GAME:
                if (UpdateGame()) state = STATE_MENU; 
                UpdateBulletPool(screenWidth, screenHeight);
                break;

            case STATE_SOBRE:
                if (UpdateSobreScreen()) state = STATE_MENU;
                break;

            case STATE_COMO_JOGAR:
                if (UpdateComoJogarScreen()) state = STATE_MENU;
                break;
            
            case STATE_EXIT:
                running = false;
                break;
        }

        BeginDrawing();
        
        switch (state) {
            case STATE_MENU:
                DrawMenu(&menu);
                break;

            case STATE_GAME:
                DrawGame();
                DrawBulletPool();
                break;

            case STATE_SOBRE:
                DrawSobreScreen();
                break;

            case STATE_COMO_JOGAR:
                DrawComoJogarScreen();
                break;
            
            case STATE_EXIT:
                break;
        }

        EndDrawing();
    }

    UnloadGame();
    UnloadMenu(&menu);
    UnloadBulletAssets();
    CloseWindow();
    return 0;
}