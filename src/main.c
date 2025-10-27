#include "raylib.h"
#include "menu.h"
#include "sobre.h"
#include "comoJogar.h"

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_SOBRE,
    STATE_COMO_JOGAR,
    STATE_EXIT
} GameState;

int main(void) {
    InitWindow(1600, 900, "SMASH TOONS");
    SetTargetFPS(60);

    Menu menu;
    InitMenu(&menu);

    GameState state = STATE_MENU;
    bool running = true;

    while (!WindowShouldClose() && running) {
        BeginDrawing();

        switch (state) {
            case STATE_MENU: {
                int selected = UpdateMenu(&menu);
                DrawMenu(&menu);
                if (selected == MENU_OPTION_PLAY) state = STATE_GAME;
                else if (selected == MENU_OPTION_ABOUT) state = STATE_SOBRE;
                else if (selected == MENU_OPTION_HOW_TO_PLAY) state = STATE_COMO_JOGAR;
                else if (selected == MENU_OPTION_EXIT) running = false;
            } break;

            case STATE_GAME:
                ClearBackground(BLACK);
                DrawText("O jogo comeca aqui...", 250, 200, 30, WHITE);
                DrawText("Pressione ESC para voltar ao menu", 200, 400, 20, GRAY);
                if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MENU;
                break;

            case STATE_SOBRE:
                DrawSobreScreen();
                if (UpdateSobreScreen()) state = STATE_MENU;
                break;

            case STATE_COMO_JOGAR:
                DrawComoJogarScreen();
                if (UpdateComoJogarScreen()) state = STATE_MENU;
                break;

            case STATE_EXIT:
                running = false;
                break;
        }

        EndDrawing();
    }

    UnloadMenu(&menu);
    CloseWindow();
    return 0;
}
