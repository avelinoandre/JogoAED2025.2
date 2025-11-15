#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include "player.h"

typedef enum {
    ITEM_MACA,
    ITEM_DINHEIRO,
    ITEM_VIDA_EXTRA
} ItemType;

void Item_Init(void);
void Item_Unload(void);
void Item_Update(Player *player);
void Item_Draw(void);
void Item_Spawn(Vector2 position, ItemType type);
void Item_DespawnAll(void);

#endif