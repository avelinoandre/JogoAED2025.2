#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h> 

typedef enum {
    CHAR_JOHNNY,
    CHAR_FINN,
    CHAR_SAMURAI,
    CHAR_MORDECAI
} CharacterType;

typedef enum {
    GAME_MODE_1P,
    GAME_MODE_2P
} GameMode;

extern CharacterType selectedCharacter;
extern CharacterType selectedCharacterP2; 
extern GameMode selectedGameMode;      

#define TOTAL_SCENES 5
extern bool sceneHasCaixa[TOTAL_SCENES + 1]; 
extern int extraLives; 

#endif