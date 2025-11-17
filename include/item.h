#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include "player.h"

typedef enum {
    ITEM_MACA,
    ITEM_DINHEIRO,
    ITEM_VIDA_EXTRA
} ItemType;

typedef struct Item {
    bool active;
    Vector2 position;
    ItemType type;
    float timer;
    Rectangle rect;
    float scale;
} Item;

void Item_Init(void);
void Item_Unload(void);
void Item_Update(Player *player1, Player *player2, bool isPlayer2Active);
void Item_Draw(void);
void Item_Spawn(Vector2 position, ItemType type);
void Item_DespawnAll(void);

#endif