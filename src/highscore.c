#include "highscore.h"
#include "score.h"
#include "globals.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define MAX_SCORES_TO_SHOW 10
#define MAX_SCORES_IN_FILE 100

static Font font;

static HighScoreEntry soloScores[MAX_SCORES_IN_FILE];
static int soloCount = 0;

static HighScoreEntry duoScores[MAX_SCORES_IN_FILE];
static int duoCount = 0;

void InsertionSortScores(HighScoreEntry arr[], int n) {
    int i, j;
    HighScoreEntry key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j].score < key.score) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void LoadAndSortScores(void) {
    FILE *file = fopen("highscores.txt", "r");
    soloCount = 0;
    duoCount = 0;

    if (file != NULL) {
        char name[4];
        int mode;
        int score;

        while (fscanf(file, "%s %d %d", name, &mode, &score) == 3) {
            if (mode == 0) { 
                if (soloCount < MAX_SCORES_IN_FILE) {
                    strcpy(soloScores[soloCount].name, name);
                    soloScores[soloCount].mode = 0;
                    soloScores[soloCount].score = score;
                    soloCount++;
                }
            } else { 
                if (duoCount < MAX_SCORES_IN_FILE) {
                    strcpy(duoScores[duoCount].name, name);
                    duoScores[duoCount].mode = 1;
                    duoScores[duoCount].score = score;
                    duoCount++;
                }
            }
        }
        fclose(file);
    }

    InsertionSortScores(soloScores, soloCount);
    InsertionSortScores(duoScores, duoCount);
}

void InitHighscoreScreen(void) {
    font = LoadFont("assets/fonts/pixelfont.ttf");
    if (font.texture.id == 0) font = GetFontDefault();
    
    LoadAndSortScores();
}

void SaveNewScore(const char* name, int mode, int score) {
    FILE *file = fopen("highscores.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %d %d\n", name, mode, score);
        fclose(file);
    }
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
    int screenHeight = GetScreenHeight();

    DrawTextEx(font, "RANKING", (Vector2){screenWidth/2 - 100, 50}, 60, 4, YELLOW);

    DrawLine(screenWidth/2, 150, screenWidth/2, screenHeight - 100, LIGHTGRAY);

    DrawTextEx(font, "SOLO (1P)", (Vector2){screenWidth/4 - 80, 120}, 40, 2, ORANGE);
    
    int yPos = 200;
    int showCount = (soloCount < MAX_SCORES_TO_SHOW) ? soloCount : MAX_SCORES_TO_SHOW;
    
    if (showCount == 0) {
        DrawText("Vazio", screenWidth/4 - 30, yPos, 30, GRAY);
    } else {
        for (int i = 0; i < showCount; i++) {
            const char *txt = TextFormat("%d. %s  %06d", i+1, soloScores[i].name, soloScores[i].score);
            DrawTextEx(font, txt, (Vector2){screenWidth/4 - 120, (float)yPos}, 30, 2, WHITE);
            yPos += 50;
        }
    }

    DrawTextEx(font, "DUPLA (2P)", (Vector2){(3*screenWidth)/4 - 80, 120}, 40, 2, GREEN);
    
    yPos = 200;
    showCount = (duoCount < MAX_SCORES_TO_SHOW) ? duoCount : MAX_SCORES_TO_SHOW;

    if (showCount == 0) {
        DrawText("Vazio", (3*screenWidth)/4 - 30, yPos, 30, GRAY);
    } else {
        for (int i = 0; i < showCount; i++) {
            const char *txt = TextFormat("%d. %s  %06d", i+1, duoScores[i].name, duoScores[i].score);
            DrawTextEx(font, txt, (Vector2){(3*screenWidth)/4 - 120, (float)yPos}, 30, 2, WHITE);
            yPos += 50;
        }
    }

    const char *prompt = "ENTER para voltar";
    DrawText(prompt, screenWidth/2 - MeasureText(prompt, 20)/2, screenHeight - 50, 20, GRAY);
}

void UnloadHighscoreScreen(void) {
    if (font.texture.id != GetFontDefault().texture.id) UnloadFont(font);
}

void RefreshHighscores(void) {
    LoadAndSortScores();
}