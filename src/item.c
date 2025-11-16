#include "item.h"
#include "player.h"
#include "score.h" 
#include "globals.h" 
#include <stdlib.h>

#define MAX_ITEMS 20

#define MACA_SCALE 1.5f
#define DINHEIRO_SCALE 3.0f
#define VIDA_EXTRA_SCALE 3.0f


typedef struct Item {
    bool active;
    Vector2 position;
    ItemType type;
} Item;

static Item itemPool[MAX_ITEMS];
static Texture2D macaTexture;
static Texture2D dinheiroTexture;
static Texture2D vidaExtraTexture;
static Sound itemPickupSound;

static Rectangle GetItemRect(const Item* item) {
    Texture2D tex;
    float scale; 

    switch (item->type) {
        case ITEM_MACA: 
            tex = macaTexture; 
            scale = MACA_SCALE;
            break;
        case ITEM_DINHEIRO: 
            tex = dinheiroTexture; 
            scale = DINHEIRO_SCALE;
            break;
        case ITEM_VIDA_EXTRA: 
            tex = vidaExtraTexture; 
            scale = VIDA_EXTRA_SCALE;
            break;
        default: return (Rectangle){0, 0, 0, 0};
    }

    return (Rectangle){
        item->position.x, item->position.y,
        (float)tex.width * scale,  
        (float)tex.height * scale 
    };
}

void Item_Init(void) {
    macaTexture = LoadTexture("assets/Sprites/itens/maca.png");
    dinheiroTexture = LoadTexture("assets/Sprites/itens/dinheiro.png");
    vidaExtraTexture = LoadTexture("assets/Sprites/itens/vidaExtra.png");

    itemPickupSound = LoadSound("assets/audios/itemSound.wav");
    SetSoundVolume(itemPickupSound, 0.8f);

    for (int i = 0; i < MAX_ITEMS; i++) {
        itemPool[i].active = false;
    }
}

void Item_Unload(void) {
    UnloadTexture(macaTexture);
    UnloadTexture(dinheiroTexture);
    UnloadTexture(vidaExtraTexture);
    UnloadSound(itemPickupSound);
}

void Item_Spawn(Vector2 position, ItemType type) {
    
    if (type == ITEM_VIDA_EXTRA) {
        bool alreadyOnGround = false;
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (itemPool[i].active && itemPool[i].type == ITEM_VIDA_EXTRA) {
                alreadyOnGround = true;
                break; 
            }
        }

        bool alreadyPickedUp = (extraLives > 1); 

        if (alreadyOnGround || alreadyPickedUp) {
 
            if (rand() % 2 == 0) {
                type = ITEM_DINHEIRO;
            } else {
                type = ITEM_MACA;
            }
        }
    }
    
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
            PlaySound(itemPickupSound); 
            switch (item->type) {
                case ITEM_MACA:
                    player->health += 50; 
                    if (player->health > player->maxHealth) {
                        player->health = player->maxHealth;
                    }
                    break;
                case ITEM_DINHEIRO:
                    Score_AddPoints(3000);
                    break;
                case ITEM_VIDA_EXTRA:
                    extraLives++;
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
        float scale;
        
        switch (item->type) {
            case ITEM_MACA: 
                tex = macaTexture; 
                scale = MACA_SCALE;
                break;
            case ITEM_DINHEIRO: 
                tex = dinheiroTexture; 
                scale = DINHEIRO_SCALE;
                break;
            case ITEM_VIDA_EXTRA: 
                tex = vidaExtraTexture; 
                scale = VIDA_EXTRA_SCALE;
                break;
            default: continue;
        }
        
        DrawTextureEx(tex, item->position, 0.0f, scale, WHITE); 
    }
}

void Item_DespawnAll(void) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        itemPool[i].active = false;
    }
}