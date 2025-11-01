#include "bullet.h"

#define BULLET_SCALE 4.0f

static Texture2D bulletTexture; 
static Bullet bulletPool[MAX_BULLETS];

void InitBulletPool(void) {

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletPool[i].active = false;
        bulletPool[i].radius = BULLET_RADIUS;
        bulletPool[i].color = YELLOW; 
    }
    
    bulletTexture = LoadTexture("assets/Sprites/BALA.png"); 

}


void DrawBulletPool(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletPool[i].active) {
            
            float scaledWidth = bulletTexture.width * BULLET_SCALE;
            float scaledHeight = bulletTexture.height * BULLET_SCALE;

            Vector2 pos = { 
                bulletPool[i].position.x - (scaledWidth / 2.0f), 
                bulletPool[i].position.y - (scaledHeight / 2.0f) 
            };

            DrawTextureEx(bulletTexture, pos, 0.0f, BULLET_SCALE, WHITE);
        }
    }
}

void SpawnBullet(Vector2 startPos, int direction) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bulletPool[i].active) {
            bulletPool[i].active = true;
            bulletPool[i].position = startPos;

            bulletPool[i].speed.x = BULLET_SPEED * direction;
            bulletPool[i].speed.y = 0; 

            return;
        }
    }
}

void UpdateBulletPool(int screenWidth, int screenHeight) {

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletPool[i].active) {

            bulletPool[i].position.x += bulletPool[i].speed.x;
            bulletPool[i].position.y += bulletPool[i].speed.y;

            if (bulletPool[i].position.x < 0 || 
                bulletPool[i].position.x > screenWidth ||
                bulletPool[i].position.y < 0 ||
                bulletPool[i].position.y > screenHeight) 
            {
                bulletPool[i].active = false;
            }
        }
    }
}

void UnloadBulletAssets(void) {
    UnloadTexture(bulletTexture);
}