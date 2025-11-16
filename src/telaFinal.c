#include "telaFinal.h"
#include "score.h"
#include "raylib.h"

static Font font;

static Music victoryMusic;

void InitTelaFinal(void) {

    font = LoadFont("assets/fonts/pixelfont.ttf");
    if (font.texture.id == 0) {
        font = GetFontDefault(); 
    }

    victoryMusic = LoadMusicStream("assets/audios/vitoria.mp3");
    victoryMusic.looping = false; 
    
    SetMusicVolume(victoryMusic, 0.4f); 
    
}

int UpdateTelaFinal(void) {
    UpdateMusicStream(victoryMusic);

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        return 1;
    }
    return 0;
}

void DrawTelaFinal(void) {
    ClearBackground((Color){30, 30, 40, 255});

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const char *title = "VOCE VENCEU!";
    Vector2 titleSize = MeasureTextEx(font, title, 80, 4);
    DrawTextEx(font, title, (Vector2){(screenWidth - titleSize.x) / 2, 120}, 80, 4, YELLOW);

    DrawText("Todos os inimigos foram derrotados!",
             (screenWidth - MeasureText("Todos os inimigos foram derrotados!", 20)) / 2, 220, 20, LIGHTGRAY);

    const char *scoreText = TextFormat("FINAL SCORE: %08i", Score_GetFinalScore());
    Vector2 scoreSize = MeasureTextEx(font, scoreText, 40, 2);
    DrawTextEx(font, scoreText, (Vector2){(screenWidth - scoreSize.x) / 2, 350}, 40, 2, WHITE);

    const char *rankText = Score_GetRank();
    Vector2 rankSize = MeasureTextEx(font, rankText, 40, 2);
    DrawTextEx(font, rankText, (Vector2){(screenWidth - rankSize.x) / 2, 410}, 40, 2, GREEN);

    const char *prompt = "Pressione ENTER para voltar ao Menu";
    Vector2 promptSize = MeasureTextEx(GetFontDefault(), prompt, 20, 0);
    DrawText(prompt, (screenWidth - promptSize.x) / 2, screenHeight - 100, 20, GRAY);
}

void UnloadTelaFinal(void) {
    if (font.texture.id != GetFontDefault().texture.id) {
        UnloadFont(font);
    }

    UnloadMusicStream(victoryMusic);
}

void PlayVictoryMusic(void) {
    StopMusicStream(victoryMusic); 
    PlayMusicStream(victoryMusic);
}

void StopVictoryMusic(void) {
    StopMusicStream(victoryMusic);
}