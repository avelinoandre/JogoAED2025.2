#include "player.h"
#include "bullet.h"
#include <stdio.h>

#define PLAYER_ANIM_SPEED_PARADO 50
#define PLAYER_ANIM_SPEED_ANDANDO 15
#define PLAYER_ANIM_SPEED_ATAQUE 30

void InitPlayer(Player *player, int startX, int startY) {
    player->position = (Vector2){ (float)startX, (float)startY };
    player->speed = 5.0f;
    player->maxHealth = 100;
    player->health = player->maxHealth;
    player->scale = 4.0f;
    player->direction = 1;
    player->isMoving = false;
    player->isAttacking = false;

    player->currentFrame = 0;
    player->framesCounter = 0;
    player->framesSpeed = PLAYER_ANIM_SPEED_PARADO;

    char path[256];

    for (int i = 0; i < PLAYER_IDLE_FRAMES; i++) {
        sprintf(path, "assets/Sprites/JohnyBravo/parado/JohnyBravo_parado%d.png", i + 1); 
        player->idleTextures[i] = LoadTexture(path);
    }

    for (int i = 0; i < PLAYER_WALK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/JohnyBravo/movimentacao/JohnyBravo_walking%d.png", i + 1); 
        player->walkTextures[i] = LoadTexture(path);
    }

    for (int i = 0; i < PLAYER_ATTACK_FRAMES; i++) {
        sprintf(path, "assets/Sprites/JohnyBravo/ataque/JohnyBravo_ataque%d.png", i + 1); 
        player->attackTextures[i] = LoadTexture(path);
    }
}

void UpdatePlayer(Player *player, int screenWidth, int screenHeight) {
    
    if (IsKeyPressed(KEY_X) && !player->isAttacking) {
        player->isAttacking = true;
        player->currentFrame = 0;
        player->framesCounter = 0;
        player->framesSpeed = PLAYER_ANIM_SPEED_ATAQUE;

        Texture2D attackTexture = player->attackTextures[0];
        float playerHeight = (float)attackTexture.height * player->scale;
        float playerWidth = (float)attackTexture.width * player->scale;

        Vector2 startPos;

        float offsetY = playerHeight * 0.38f; 
        float offsetX_Right = playerWidth * 1.0f;
        float offsetX_Left = playerWidth * 0.2f; 
        
        startPos.y = player->position.y + offsetY;
        
        if (player->direction == 1) { 
            startPos.x = player->position.x + offsetX_Right;
        } else { 
            startPos.x = player->position.x + offsetX_Left;
        }
        
        SpawnBullet(startPos, player->direction);
    }

    bool wasMoving = player->isMoving;
    player->isMoving = false;
    
    if (!player->isAttacking) {
        if (IsKeyDown(KEY_W)) { player->position.y -= player->speed; player->isMoving = true; }
        if (IsKeyDown(KEY_S)) { player->position.y += player->speed; player->isMoving = true; }
        if (IsKeyDown(KEY_A)) { player->position.x -= player->speed; player->isMoving = true; player->direction = -1; }
        if (IsKeyDown(KEY_D)) { player->position.x += player->speed; player->isMoving = true; player->direction = 1; }
    }

    int currentAnimFrameCount = 0;
    
    if (player->isAttacking) {

        currentAnimFrameCount = PLAYER_ATTACK_FRAMES;
        
        player->framesCounter++;
        if (player->framesCounter >= player->framesSpeed) {
            player->framesCounter = 0;
            player->currentFrame++;
            
            if (player->currentFrame >= currentAnimFrameCount) {
                player->isAttacking = false;
                player->currentFrame = 0;
            }
        }
    } 

    else {

        if (player->isMoving) {
            currentAnimFrameCount = PLAYER_WALK_FRAMES;
        } else {
            currentAnimFrameCount = PLAYER_IDLE_FRAMES;
        }

        if (player->isMoving != wasMoving) {
            player->currentFrame = 0;
            player->framesCounter = 0;
            player->framesSpeed = player->isMoving ? PLAYER_ANIM_SPEED_ANDANDO : PLAYER_ANIM_SPEED_PARADO;
        }
        
        player->framesCounter++;
        if (player->framesCounter >= player->framesSpeed) {
            player->framesCounter = 0;
            player->currentFrame++;
            
            if (player->currentFrame >= currentAnimFrameCount) {
                player->currentFrame = 0;
            }
        }
    }

    Texture2D currentTexture;
    if (player->isAttacking) {
        currentTexture = player->attackTextures[player->currentFrame];
    } else if (player->isMoving) {
        currentTexture = player->walkTextures[player->currentFrame];
    } else {
        currentTexture = player->idleTextures[player->currentFrame];
    }

    float playerWidth = (float)currentTexture.width * player->scale;
    float playerHeight = (float)currentTexture.height * player->scale;

    if (player->position.x < 0) player->position.x = 0;
    if (player->position.y < 0) player->position.y = 0;
    if (player->position.x + playerWidth > screenWidth) player->position.x = screenWidth - playerWidth;
    if (player->position.y + playerHeight > screenHeight) player->position.y = screenHeight - playerHeight;

    if (IsKeyPressed(KEY_SPACE)) {
        player->health -= 20;
        if (player->health < 0) player->health = 0;
    }
}

void DrawPlayer(const Player *player) {
    
    Texture2D textureToDraw;

    if (player->isAttacking) {
        textureToDraw = player->attackTextures[player->currentFrame];
    } else if (player->isMoving) {
        textureToDraw = player->walkTextures[player->currentFrame];
    } else {
        textureToDraw = player->idleTextures[player->currentFrame];
    }
    
    Rectangle sourceRec = { 0.0f, 0.0f, (float)textureToDraw.width, (float)textureToDraw.height };
    
    if (player->direction == -1) {
        sourceRec.width = -sourceRec.width;
    }
    
    Rectangle destRec = {
        player->position.x,
        player->position.y,
        (float)textureToDraw.width * player->scale,
        (float)textureToDraw.height * player->scale
    };
    
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(textureToDraw, sourceRec, destRec, origin, 0.0f, WHITE);
}
    
void DrawPlayerHealthBar(const Player *player) {
    int barWidth = 200;
    int barHeight = 20;
    int barX = 20;
    int barY = 20;
    
    float healthPercentage = (float)player->health / (float)player->maxHealth;
    int currentHealthWidth = (int)(barWidth * healthPercentage);
    
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, currentHealthWidth, barHeight, GREEN);
    DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);
}

Texture2D GetPlayerCurrentTexture(const Player *player) {
    if (player->isAttacking) {
        return player->attackTextures[player->currentFrame];
    } else if (player->isMoving) {
        return player->walkTextures[player->currentFrame];
    } else {
        return player->idleTextures[player->currentFrame];
    }
}

void UnloadPlayer(Player *player) {

    for (int i = 0; i < PLAYER_IDLE_FRAMES; i++) {
        UnloadTexture(player->idleTextures[i]);
    }
    for (int i = 0; i < PLAYER_WALK_FRAMES; i++) {
        UnloadTexture(player->walkTextures[i]);
    }
    for (int i = 0; i < PLAYER_ATTACK_FRAMES; i++) {
        UnloadTexture(player->attackTextures[i]);
    }

}