#include "player.h"

void InitPlayer(Player *player, int startX, int startY) {
    player->position = (Vector2){ (float)startX, (float)startY };
    player->speed = 5.0f;
    player->maxHealth = 100;
    player->health = player->maxHealth;
    
    player->texture = LoadTexture("assets/Sprites/JohnyBravo/Johnymovimentacao/JohnyBravo7.png");
    player->scale = 4.0f;
    
    player->currentFrame = 0;
    player->framesCounter = 0;
    player->direction = 1;
    player->isMoving = false;
}

void UpdatePlayer(Player *player, int screenWidth, int screenHeight) {
    
    player->isMoving = false;

    if (IsKeyDown(KEY_W)) {
        player->position.y -= player->speed;
        player->isMoving = true;
    }
    if (IsKeyDown(KEY_S)) {
        player->position.y += player->speed;
        player->isMoving = true;
    }
    if (IsKeyDown(KEY_A)) {
        player->position.x -= player->speed;
        player->isMoving = true;
        player->direction = -1;
    }
    if (IsKeyDown(KEY_D)) {
        player->position.x += player->speed;
        player->isMoving = true;
        player->direction = 1;
    }

    if (player->isMoving) {
        player->framesCounter++;
        if (player->framesCounter >= 5) {
            player->framesCounter = 0;
            player->currentFrame++;
            if (player->currentFrame > 5) {
                player->currentFrame = 0;
            }
        }
    } else {
        player->currentFrame = 0;
        player->framesCounter = 0;
    }
    
    float playerWidth = ((float)player->texture.width / 6.0f) * player->scale;
    float playerHeight = (float)player->texture.height * player->scale;

    if (player->position.x < 0) {
        player->position.x = 0;
    }
    if (player->position.y < 0) {
        player->position.y = 0;
    }
    if (player->position.x + playerWidth > screenWidth) {
        player->position.x = screenWidth - playerWidth;
    }
    if (player->position.y + playerHeight > screenHeight) {
        player->position.y = screenHeight - playerHeight;
    }
    
    if (IsKeyPressed(KEY_SPACE)) {
        player->health -= 20;
        if (player->health < 0) {
            player->health = 0;
        }
    }
}

void DrawPlayer(const Player *player) {
    float frameWidth = (float)player->texture.width;
    float frameHeight = (float)player->texture.height;

    Rectangle sourceRec;
    sourceRec.y = 0.0f;
    sourceRec.height = frameHeight;

    if (player->direction == 1) {
        sourceRec.x = frameWidth * player->currentFrame;
        sourceRec.width = frameWidth;
    } else {
        sourceRec.x = frameWidth * (player->currentFrame + 1);
        sourceRec.width = -frameWidth;
    }
    
    Rectangle destRec = {
        player->position.x,
        player->position.y,
        frameWidth * player->scale,
        frameHeight * player->scale
    };
    
    Vector2 origin = { 0.0f, 0.0f };
    
    DrawTexturePro(player->texture, sourceRec, destRec, origin, 0.0f, WHITE);
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

void UnloadPlayer(Player *player) {
    UnloadTexture(player->texture);
}