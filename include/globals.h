#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h> 

typedef enum {
    CHAR_JOHNNY,
    CHAR_FINN,
    CHAR_SAMURAI,
    CHAR_MORDECAI
} CharacterType;

extern CharacterType selectedCharacter;

#define TOTAL_SCENES 5
extern bool sceneHasCaixa[TOTAL_SCENES + 1]; 
extern int extraLives; 

#endif