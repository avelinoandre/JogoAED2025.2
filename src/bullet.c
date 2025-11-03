#include "bullet.h"
#include "raylib.h" 

#define BULLET_SCALE 4.0f
#define MAX_AMMO 10       
#define RELOAD_TIME 3.0f  

static Texture2D bulletTexture; 
static Bullet bulletPool[MAX_BULLETS];

static int currentAmmo;
static bool isReloading;
static float reloadTimer;

void InitBulletPool(void) {

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletPool[i].active = false;
        bulletPool[i].radius = BULLET_RADIUS;
        bulletPool[i].color = YELLOW; 
    }
    
    bulletTexture = LoadTexture("assets/Sprites/BALA.png"); 

    currentAmmo = MAX_AMMO;
    isReloading = false;
    reloadTimer = 0.0f;
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
    
    if (isReloading) {
        return;
    }

    if (currentAmmo > 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bulletPool[i].active) {
                bulletPool[i].active = true;
                bulletPool[i].position = startPos;

                bulletPool[i].speed.x = BULLET_SPEED * direction;
                bulletPool[i].speed.y = 0; 

                currentAmmo--; 
                
                if (currentAmmo == 0) {
                    isReloading = true;
                    reloadTimer = RELOAD_TIME;
                }

                return; 
            }
        }
    }
    else if (!isReloading) 
    {
        isReloading = true;
        reloadTimer = RELOAD_TIME;
    }
}

void UpdateBulletPool(int screenWidth, int screenHeight) {

    if (isReloading) {
        reloadTimer -= GetFrameTime(); 
        
        if (reloadTimer <= 0) {
            isReloading = false;
            currentAmmo = MAX_AMMO; 
            reloadTimer = 0.0f;
        }
    }

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

int GetCurrentAmmo(void) {
    return currentAmmo;
}

bool IsReloading(void) {
    return isReloading;
}

void DrawAmmoCount(void) {
    int posX = 20;
    int posY = 20 + 20 + 10; 
    int fontSize = 20;

    if (IsReloading()) {
        DrawText("RECARREGANDO...", posX, posY, fontSize, RED);
    } else {
        DrawText(TextFormat("Balas: %d / %d", GetCurrentAmmo(), MAX_AMMO), posX, posY, fontSize, WHITE);
    }
}