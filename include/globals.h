#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h> // Adicionado para usar o tipo bool

typedef enum {
    CHAR_JOHNNY,
    CHAR_FINN,
    CHAR_SAMURAI,
    CHAR_MORDECAI
} CharacterType;

extern CharacterType selectedCharacter;

#define TOTAL_SCENES 5
extern bool sceneHasCaixa[TOTAL_SCENES + 1]; // +1 para indexação base 1 (Cenas 1-5)
extern int extraLives; // Contador de vidas extras

#endif