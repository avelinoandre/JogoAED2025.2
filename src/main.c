#include "raylib.h"
#include "menu.h"
#include "sobre.h"
#include "comoJogar.h"
#include "jogo.h" 
#include "bullet.h" 
#include "globals.h"      
#include "char_select.h"  
#include <curl/curl.h>

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_SOBRE,
    STATE_COMO_JOGAR,
    STATE_CHAR_SELECT, 
    STATE_EXIT
} GameState;

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    curl_global_init(CURL_GLOBAL_ALL);

    InitWindow(screenWidth, screenHeight, "SMASH TOONS");
    InitAudioDevice();
    SetTargetFPS(60);

    Menu menu;
    InitMenu(&menu);

    InitCharSelectMenu(); 
    
    GameState state = STATE_MENU;
    bool running = true;

    while (!WindowShouldClose() && running) {
        
        switch (state) {
            case STATE_MENU: {
                int selected = UpdateMenu(&menu);
                if (selected == MENU_OPTION_PLAY) state = STATE_CHAR_SELECT; 
                else if (selected == MENU_OPTION_ABOUT) state = STATE_SOBRE;
                else if (selected == MENU_OPTION_HOW_TO_PLAY) state = STATE_COMO_JOGAR;
                else if (selected == MENU_OPTION_EXIT) running = false;
            } break;

            case STATE_GAME:
                {
                    int gameResult = UpdateGame(); 
                    
                    if (gameResult == 1 || gameResult == 2) { 
                        UnloadGame();       
                        state = STATE_MENU; 
                    }
                }
                break;

            case STATE_SOBRE:
                if (UpdateSobreScreen()) state = STATE_MENU;
                break;

            case STATE_COMO_JOGAR:
                if (UpdateComoJogarScreen()) state = STATE_MENU;
                break;
            
            case STATE_CHAR_SELECT:
                { 
                    GameState oldState = state; 
                    
                    UpdateCharSelectMenu((int*)&state); 

                    if (oldState == STATE_CHAR_SELECT && state == STATE_GAME) {
                        InitGame();    
                    }
                }
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
                break;

            case STATE_SOBRE:
                DrawSobreScreen();
                break;

            case STATE_COMO_JOGAR:
                DrawComoJogarScreen();
                break;
            
            case STATE_CHAR_SELECT:
                DrawCharSelectMenu();
                break;

            case STATE_EXIT:
                break;
        }

        EndDrawing();
    }

    UnloadMenu(&menu);
    UnloadCharSelectMenu(); 
    CloseAudioDevice();
    CloseWindow();
    
    curl_global_cleanup();

    return 0;
}