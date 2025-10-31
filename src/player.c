#include "player.h"
#include "animation.h"

#define PLAYER_WALK_FRAMES 6
#define PLAYER_ANIM_FPS 10

void InitPlayer(Player *player, int startX, int startY) {
    player->position = (Vector2){ (float)startX, (float)startY };
    player->speed = 5.0f;
    player->maxHealth = 100;
    player->health = player->maxHealth;
    player->scale = 4.0f;
    player->direction = 1;
    player->isMoving = false;

    player->atlas = LoadTexture("assets/Sprites/JohnyBravo/JohnyBravo.png");

    player->frameWidth = 35.0f; 
    player->frameHeight = 46.0f; 

    Rectangle idleRects[] = {
        { 0.0f, 0.0f, player->frameWidth, player->frameHeight }
    };
    player->animIdle = CreateSpriteAnimation(player->atlas, PLAYER_ANIM_FPS, idleRects, 1);

    float x_offset = 1 * player->frameWidth; //COLUNA DE ANIMAÇÃO
    
    Rectangle walkRects[PLAYER_WALK_FRAMES];
    for (int i = 0; i < PLAYER_WALK_FRAMES; i++) {
        walkRects[i] = (Rectangle){ 
            x_offset,
            i * player->frameHeight,
            player->frameWidth,
            player->frameHeight
        };
    }

    player->animWalk = CreateSpriteAnimation(player->atlas, PLAYER_ANIM_FPS, walkRects, PLAYER_WALK_FRAMES);
    
}

void UpdatePlayer(Player *player, int screenWidth, int screenHeight) {
    
    bool wasMoving = player->isMoving;
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

    if (player->isMoving && !wasMoving) {
        player->animWalk.timeStarted = GetTime();
    }
    
    if (!player->isMoving && wasMoving) {
        player->animIdle.timeStarted = GetTime();
    }

    float playerWidth = player->frameWidth * player->scale;
    float playerHeight = player->frameHeight * player->scale;

    if (player->position.x < 0) player->position.x = 0;
    if (player->position.y < 0) player->position.y = 0;
    if (player->position.x + playerWidth > screenWidth) player->position.x = screenWidth - playerWidth;
    if (player->position.y + playerHeight > screenHeight) player->position.y = screenHeight - playerHeight;
    
    if (IsKeyPressed(KEY_SPACE)) {
        player->health -= 20;
        if (player->health < 0) {
            player->health = 0;
        }
    }
}

void DrawPlayer(const Player *player) {
    
    const SpriteAnimation* currentAnim;
    if (player->isMoving) {
        currentAnim = &player->animWalk;
    } else {
        currentAnim = &player->animIdle;
    }

    int index = (int)((GetTime() - currentAnim->timeStarted) * currentAnim->framesPerSecond) % currentAnim->rectanglesLength;
    Rectangle sourceRec = currentAnim->rectangles[index];

    if (player->direction == -1) {
        sourceRec.width = -sourceRec.width;
    }

    Rectangle destRec = {
        player->position.x,
        player->position.y,
        player->frameWidth * player->scale,
        player->frameHeight * player->scale
    };
    
    Vector2 origin = { 0.0f, 0.0f };
    
    DrawTexturePro(player->atlas, sourceRec, destRec, origin, 0.0f, WHITE);
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

    DisposeSpriteAnimation(player->animIdle);
    DisposeSpriteAnimation(player->animWalk);
    UnloadTexture(player->atlas);

}