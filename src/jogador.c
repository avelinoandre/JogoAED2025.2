#include "player.h"

void InitPlayer(Player *player, int startX, int startY) {
    player->position = (Vector2){ (float)startX, (float)startY };
    player->speed = 5.0f;
    player->size = (Vector2){ 50, 80 };
    player->maxHealth = 100;
    player->health = player->maxHealth;
}

void UpdatePlayer(Player *player, int screenWidth, int screenHeight) {
    
    if (IsKeyDown(KEY_W)) {
        player->position.y -= player->speed;
    }
    if (IsKeyDown(KEY_S)) {
        player->position.y += player->speed;
    }
    if (IsKeyDown(KEY_A)) {
        player->position.x -= player->speed;
    }
    if (IsKeyDown(KEY_D)) {
        player->position.x += player->speed;
    }

    if (player->position.x < 0) {
        player->position.x = 0;
    }
    if (player->position.y < 0) {
        player->position.y = 0;
    }
    if (player->position.x + player->size.x > screenWidth) {
        player->position.x = screenWidth - player->size.x;
    }
    if (player->position.y + player->size.y > screenHeight) {
        player->position.y = screenHeight - player->size.y;
    }
    
    if (IsKeyPressed(KEY_SPACE)) {
        player->health -= 20;
        if (player->health < 0) {
            player->health = 0;
        }
    }
}

void DrawPlayer(const Player *player) {
    DrawRectangleV(player->position, player->size, RED);
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