#include "mapa.h"
#include <stdlib.h> 
#include <stdio.h> 

static const int screenWidth = 1600;
static const int screenHeight = 900;
static SceneNode* firstScene = NULL;  
static SceneNode* currentScene = NULL; 

SceneNode* CreateScene(const char* backgroundPath, SceneNode* previousNode) {
    SceneNode* newScene = (SceneNode*)malloc(sizeof(SceneNode));
    if (newScene == NULL) {
        printf("ERRO: Falha ao alocar memória para a cena.\n");
        return NULL;
    }

    newScene->background = LoadTexture(backgroundPath);
    if (newScene->background.id <= 0) {
        printf("AVISO: Falha ao carregar textura: %s\n", backgroundPath);
    }
    
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
    SceneNode* scene1 = CreateScene("assets/Sprites/map/cidade/cidade00.png", NULL);
    firstScene = scene1;
    
    SceneNode* scene2 = CreateScene("assets/Sprites/map/cidade/cidade01.png", scene1);
    
    SceneNode* scene3 = CreateScene("assets/Sprites/map/cidade/cidade02.png", scene2);
    
    SceneNode* scene4 = CreateScene("assets/Sprites/map/cidade/cidade03.png", scene3);
    SceneNode* scene5 = CreateScene("assets/Sprites/map/cidade/cidade04.png", scene4);
    SceneNode* scene6 = CreateScene("assets/Sprites/map/cidade/cidade05.png", scene5);
    SceneNode* scene7 = CreateScene("assets/Sprites/map/cidade/cidade06.png", scene6);
    SceneNode* scene8 = CreateScene("assets/Sprites/map/cidade/cidade07.png", scene7);
    SceneNode* scene9 = CreateScene("assets/Sprites/map/cidade/cidade08.png", scene8);
    SceneNode* scene10 = CreateScene("assets/Sprites/map/cidade/cidade09.png", scene9);
    SceneNode* scene11 = CreateScene("assets/Sprites/map/cidade/cidade10.png", scene10);
    SceneNode* scene12 = CreateScene("assets/Sprites/map/cidade/cidade11.png", scene11); 

    scene1->enemyCount = 3; 
    scene2->enemyCount = 2;
    scene3->enemyCount = 5; 
    scene4->enemyCount = 3; 
    scene5->enemyCount = 4; 
    scene6->enemyCount = 6; 
    scene7->enemyCount = 3; 
    scene8->enemyCount = 4; 
    scene9->enemyCount = 2; 
    scene10->enemyCount = 6; 
    scene11->enemyCount = 5; 
    scene12->enemyCount = 10; 

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