#include "highscore.h"
#include "score.h"
#include "raylib.h"
#include <stdio.h> 

#define MAX_SCORES_TO_SHOW 10
#define MAX_SCORES_IN_MEMORY 100 

static Font font;
static int highScores[MAX_SCORES_IN_MEMORY];
static int scoreCount = 0;

void InitHighscoreScreen(void) {
    font = LoadFont("assets/fonts/pixelfont.ttf");
    if (font.texture.id == 0) {
        font = GetFontDefault();
    }

    scoreCount = Score_LoadHighScores(highScores, MAX_SCORES_IN_MEMORY);

    Score_SortHighScores(highScores, scoreCount);
}

int UpdateHighscoreScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        return 1;
    }
    return 0; 
}

void DrawHighscoreScreen(void) {
    ClearBackground((Color){30, 30, 40, 255}); 
    int screenWidth = GetScreenWidth();

    const char *title = "RECORDES";
    Vector2 titleSize = MeasureTextEx(font, title, 80, 4);
    DrawTextEx(font, title, (Vector2){(screenWidth - titleSize.x) / 2, 80}, 80, 4, YELLOW);

    if (scoreCount == 0) {
        DrawText("Nenhum score registrado ainda!", (screenWidth - MeasureText("Nenhum score registrado ainda!", 20)) / 2, 250, 20, LIGHTGRAY);
    } else {
        int yPos = 200;
        int scoresToShow = (scoreCount < MAX_SCORES_TO_SHOW) ? scoreCount : MAX_SCORES_TO_SHOW;

        for (int i = 0; i < scoresToShow; i++) {
            const char *scoreText = TextFormat("%2d. %08i", i + 1, highScores[i]);
            Vector2 scoreSize = MeasureTextEx(font, scoreText, 40, 2);
            DrawTextEx(font, scoreText, (Vector2){(screenWidth - scoreSize.x) / 2, (float)yPos}, 40, 2, WHITE);
            yPos += 50;
        }
    }

    const char *prompt = "Pressione ENTER para voltar ao Menu";
    Vector2 promptSize = MeasureTextEx(GetFontDefault(), prompt, 20, 0);
    DrawText(prompt, (screenWidth - promptSize.x) / 2, GetScreenHeight() - 70, 20, GRAY);
}

void UnloadHighscoreScreen(void) {
    if (font.texture.id != GetFontDefault().texture.id) {
        UnloadFont(font);
    }
}

void RefreshHighscores(void) {
    scoreCount = Score_LoadHighScores(highScores, MAX_SCORES_IN_MEMORY);

    Score_SortHighScores(highScores, scoreCount);
}