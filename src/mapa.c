#include "mapa.h"
#include <stdlib.h> 
#include <stdio.h> 

static const int screenWidth = 1600;
static const int screenHeight = 900;
static SceneNode* firstScene = NULL;  
static SceneNode* currentScene = NULL; 

SceneNode* CreateScene(const char* backgroundPath, SceneNode* previousNode, int sceneId) {
    SceneNode* newScene = (SceneNode*)malloc(sizeof(SceneNode));
    if (newScene == NULL) {
        printf("ERRO: Falha ao alocar memória para a cena.\n");
        return NULL;
    }

    newScene->background = LoadTexture(backgroundPath);
    if (newScene->background.id <= 0) {
        printf("AVISO: Falha ao carregar textura: %s\n", backgroundPath);
    }
    
    newScene->id = sceneId; 
    newScene->previous = previousNode;
    newScene->next = NULL;        

    newScene->enemyCount = 0; 
    newScene->enemiesSpawned = false;
    newScene->isCleared = false;
    
    if (previousNode != NULL) {
        previousNode->next = newScene;
    }

    return newScene;
}

void InitMap(void) {
    SceneNode* scene1 = CreateScene("assets/Sprites/map/cidade/cidade02.png", NULL, 1);
    firstScene = scene1;
    
    SceneNode* scene2 = CreateScene("assets/Sprites/map/cidade/cidade03.png", scene1, 2);
    SceneNode* scene3 = CreateScene("assets/Sprites/map/cidade/cidade04.png", scene2, 3);
    SceneNode* scene4 = CreateScene("assets/Sprites/map/cidade/cidade05.png", scene3, 4);
    SceneNode* scene5 = CreateScene("assets/Sprites/map/cidade/cidade06.png", scene4, 5);
     


    currentScene = firstScene;
}

void UnloadMap(void) {
    SceneNode* currentNode = firstScene;
    
    while (currentNode != NULL) {
        SceneNode* nextNode = currentNode->next; 
        
        UnloadTexture(currentNode->background);
        free(currentNode);
        
        currentNode = nextNode;
    }
    
    firstScene = NULL;
    currentScene = NULL;
}

void DrawCurrentMap(void) {
    if (currentScene != NULL) {
        
        Rectangle sourceRec = { 
            0.0f, 0.0f, 
            (float)currentScene->background.width, 
            (float)currentScene->background.height 
        };
        
        Rectangle destRec = { 
            0.0f, 0.0f, 
            (float)screenWidth, 
            (float)screenHeight
        };
        
        Vector2 origin = { 0.0f, 0.0f };
        
        DrawTexturePro(
            currentScene->background, 
            sourceRec, 
            destRec, 
            origin, 
            0.0f, 
            WHITE
        );
    }
}

SceneNode* GetCurrentScene(void) {
    return currentScene;
}

void SetCurrentScene(SceneNode* newScene) {
    if (newScene != NULL) {
        currentScene = newScene;
    }
}