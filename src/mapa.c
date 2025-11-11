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
    SceneNode* scene1 = CreateScene("assets/Sprites/map/cidade/cidade00.png", NULL, 1);
    firstScene = scene1;
    
    SceneNode* scene2 = CreateScene("assets/Sprites/map/cidade/cidade01.png", scene1, 2);
    SceneNode* scene3 = CreateScene("assets/Sprites/map/cidade/cidade02.png", scene2, 3);
    SceneNode* scene4 = CreateScene("assets/Sprites/map/cidade/cidade03.png", scene3, 4);
    SceneNode* scene5 = CreateScene("assets/Sprites/map/cidade/cidade04.png", scene4, 5);
    SceneNode* scene6 = CreateScene("assets/Sprites/map/cidade/cidade05.png", scene5, 6);
    SceneNode* scene7 = CreateScene("assets/Sprites/map/cidade/cidade06.png", scene6, 7);
    SceneNode* scene8 = CreateScene("assets/Sprites/map/cidade/cidade07.png", scene7, 8);
    SceneNode* scene9 = CreateScene("assets/Sprites/map/cidade/cidade08.png", scene8, 9);
    SceneNode* scene10 = CreateScene("assets/Sprites/map/cidade/cidade09.png", scene9, 10);
    SceneNode* scene11 = CreateScene("assets/Sprites/map/cidade/cidade10.png", scene10, 11);
    SceneNode* scene12 = CreateScene("assets/Sprites/map/cidade/cidade11.png", scene11, 12); 


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