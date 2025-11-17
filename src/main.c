#include "raylib.h"
#include "menu.h"
#include "sobre.h"
#include "comoJogar.h"
#include "jogo.h" 
#include "bullet.h" 
#include "globals.h"      
#include "telaFinal.h"
#include "char_select.h"  
#include "score.h"
#include "highscore.h"
#include "mode_select.h" 
#include <curl/curl.h> // Para a IA

#include "name_input.h" // Para a tela de nome

// Enumeração de todos os estados (telas) do jogo
typedef enum {
    STATE_MENU,
    STATE_MODE_SELECT,
    STATE_NAME_INPUT,
    STATE_CHAR_SELECT, 
    STATE_GAME,
    STATE_SOBRE,
    STATE_COMO_JOGAR,
    STATE_FINAL,
    STATE_RANKING,
    STATE_EXIT
} GameState;

/*
  Função principal do programa.
  Inicializa a janela, o áudio e a música do menu.
  Contém o loop principal (game loop) e a máquina de estados (switch)
  que controla qual tela (estado) está ativa e sendo desenhada.
 */
int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    curl_global_init(CURL_GLOBAL_ALL); // Inicializa a biblioteca cURL (para a API Gemini)

    InitWindow(screenWidth, screenHeight, "SMASH TOONS");
    InitAudioDevice();
    SetTargetFPS(60);

    Music menuMusic = LoadMusicStream("assets/audios/menuSoundtrack1.mp3");
    SetMusicVolume(menuMusic, 0.5f);
    PlayMusicStream(menuMusic);

    // Inicializa todas as telas
    Menu menu;
    InitMenu(&menu);
    InitTelaFinal();
    InitModeSelect(); 
    InitNameInput();
    InitCharSelectMenu(); 
    InitHighscoreScreen();
    
    GameState state = STATE_MENU;
    bool running = true;

    // Game Loop Principal
    while (!WindowShouldClose() && running) {

        UpdateMusicStream(menuMusic);
        
        // Update das telas
        switch (state) {
            case STATE_MENU: {
                int selected = UpdateMenu(&menu);
                if (selected == MENU_OPTION_PLAY) state = STATE_MODE_SELECT; 
                else if (selected == MENU_OPTION_RANKING) {
                    RefreshHighscores();
                    state = STATE_RANKING;
                }
                else if (selected == MENU_OPTION_ABOUT) state = STATE_SOBRE;
                else if (selected == MENU_OPTION_HOW_TO_PLAY) state = STATE_COMO_JOGAR;
                else if (selected == MENU_OPTION_EXIT) running = false;
            } break;

            case STATE_MODE_SELECT:
                {
                    int modeResult = UpdateModeSelect();
                    if (modeResult != 0 && modeResult != -1) { // 1P ou 2P selecionado
                        state = STATE_NAME_INPUT;
                    } else if (modeResult == 0) { // ESC pressionado
                        state = STATE_MENU;
                    }
                }
                break;
            
            case STATE_NAME_INPUT:
                {
                    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
                        state = STATE_MODE_SELECT;
                    }
                    else if (UpdateNameInput() == 1) { // Nome inserido
                        state = STATE_CHAR_SELECT;
                    }
                }
                break;

            case STATE_GAME:
                {
                    int gameResult = UpdateGame(); 
                    
                    if (gameResult == 1) { // Morreu e voltou ao menu
                        UnloadGame();    
                        StopMusicStream(menuMusic);
                        PlayMusicStream(menuMusic);
                        state = STATE_MENU; 
                    } else if (gameResult == 2) { // Venceu o jogo
                        
                        Score_CalculateFinal();
                        int finalScore = Score_GetFinalScore();
                        int currentMode = (selectedGameMode == GAME_MODE_1P) ? 0 : 1;
                        SaveNewScore(currentPlayerName, currentMode, finalScore); // Salva no ranking

                        UnloadGame();
                        PauseMusicStream(menuMusic);
                        PlayVictoryMusic();
                        state = STATE_FINAL;
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
                    
                    UpdateCharSelectMenu((int*)&state); // Passa o estado para ser modificado

                    // Se a seleção de personagem terminou e o estado mudou para "GAME"
                    if (oldState == STATE_CHAR_SELECT && state == STATE_GAME) {
                        PauseMusicStream(menuMusic);
                        InitGame(); // Inicializa o jogo
                    }
                }
                break;
            
            case STATE_FINAL: // Tela de Vitória
                if (UpdateTelaFinal() == 1) { 
                    StopVictoryMusic();
                    StopMusicStream(menuMusic);
                    PlayMusicStream(menuMusic);
                    state = STATE_MENU; 
                }
                break;
            
            case STATE_RANKING: // Tela de High Scores
                if (UpdateHighscoreScreen() == 1) {
                    state = STATE_MENU;
                }
                break;

            case STATE_EXIT:
                running = false;
                break;
        }

        // Desenho das telas
        BeginDrawing();
        
        switch (state) {
            case STATE_MENU:
                DrawMenu(&menu);
                break;
            
            case STATE_MODE_SELECT: 
                DrawModeSelect();
                break;
            
            case STATE_NAME_INPUT: 
                DrawNameInput();
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
            
            case STATE_FINAL:
                DrawTelaFinal();
                break;
            
            case STATE_RANKING:
                DrawHighscoreScreen();
                break;

            case STATE_EXIT:
                break;
        }

        EndDrawing();
    }

    // Fechando e descarregando tudo usado no jogo
    UnloadMenu(&menu);
    UnloadModeSelect(); 
    UnloadCharSelectMenu(); 
    UnloadNameInput();
    UnloadTelaFinal();
    UnloadHighscoreScreen();
    UnloadMusicStream(menuMusic);
    CloseAudioDevice();
    CloseWindow();
    
    curl_global_cleanup(); // Limpa a biblioteca cURL

    return 0;
}