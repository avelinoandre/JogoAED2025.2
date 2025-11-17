#ifndef MAPA_H_
#define MAPA_H_

#include "raylib.h"

#define RUA_LIMITE_SUPERIOR 450.0f
typedef struct SceneNode {
    Texture2D background;
    struct SceneNode* next;
    struct SceneNode* previous;

    int id; 

    int enemyCount;  
    bool enemiesSpawned; 
    bool isCleared;
} SceneNode;

void InitMap(void);
void UnloadMap(void);
void DrawCurrentMap(void);
SceneNode* GetCurrentScene(void);
void SetCurrentScene(SceneNode* newScene);

#endif