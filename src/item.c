#include "item.h"
#include "player.h"
#include "score.h" 
#include <stdlib.h>

#define MAX_ITEMS 20
#define ITEM_SCALE 3.0f

typedef struct Item {
    bool active;
    Vector2 position;
    ItemType type;
} Item;

static Item itemPool[MAX_ITEMS];
static Texture2D macaTexture;
static Texture2D dinheiroTexture;
static Texture2D vidaExtraTexture;

static Rectangle GetItemRect(const Item* item) {
    Texture2D tex;
    switch (item->type) {
        case ITEM_MACA: tex = macaTexture; break;
        case ITEM_DINHEIRO: tex = dinheiroTexture; break;
        case ITEM_VIDA_EXTRA: tex = vidaExtraTexture; break;
        default: return (Rectangle){0, 0, 0, 0};
    }
    return (Rectangle){
        item->position.x, item->position.y,
        (float)tex.width * ITEM_SCALE,
        (float)tex.height * ITEM_SCALE
    };
}

void Item_Init(void) {
    macaTexture = LoadTexture("assets/Sprites/itens/maca.png");
    dinheiroTexture = LoadTexture("assets/Sprites/itens/dinheiro.png");
    vidaExtraTexture = LoadTexture("assets/Sprites/itens/vidaExtra.png");

    for (int i = 0; i < MAX_ITEMS; i++) {
        itemPool[i].active = false;
    }
}

void Item_Unload(void) {
    UnloadTexture(macaTexture);
    UnloadTexture(dinheiroTexture);
    UnloadTexture(vidaExtraTexture);
}

void Item_Spawn(Vector2 position, ItemType type) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!itemPool[i].active) {
            itemPool[i].active = true;
            itemPool[i].position = position;
            itemPool[i].type = type;
            return;
        }
    }
}

void Item_Update(Player *player) {
    Rectangle playerRect = GetPlayerRect(player);

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!itemPool[i].active) continue;

        Item *item = &itemPool[i];
        Rectangle itemRect = GetItemRect(item);

        if (CheckCollisionRecs(playerRect, itemRect)) {
            switch (item->type) {
                case ITEM_MACA:
                    player->health += 30; 
                    if (player->health > player->maxHealth) {
                        player->health = player->maxHealth;
                    }
                    break;
                case ITEM_DINHEIRO:
                    Score_AddPoints(500);
                    break;
                case ITEM_VIDA_EXTRA:
                    player->health = player->maxHealth; 
                    break;
            }
            item->active = false;
        }
    }
}

void Item_Draw(void) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!itemPool[i].active) continue;

        Item *item = &itemPool[i];
        Texture2D tex;
        
        switch (item->type) {
            case ITEM_MACA: tex = macaTexture; break;
            case ITEM_DINHEIRO: tex = dinheiroTexture; break;
            case ITEM_VIDA_EXTRA: tex = vidaExtraTexture; break;
            default: continue;
        }
        
        DrawTextureEx(tex, item->position, 0.0f, ITEM_SCALE, WHITE);
    }
}

void Item_DespawnAll(void) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        itemPool[i].active = false;
    }
}