#ifndef HIGHSCORE_H
#define HIGHSCORE_H

void InitHighscoreScreen(void);
int UpdateHighscoreScreen(void);
void DrawHighscoreScreen(void);
void UnloadHighscoreScreen(void);
void RefreshHighscores(void);
void SaveNewScore(const char* name, int mode, int score);

#endif 