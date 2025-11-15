#include "bullet.h"
#include "raylib.h" 
#include "raymath.h"

#define BULLET_SCALE 4.0f
#define MAX_AMMO 10
#define BULLET_DAMAGE 25

static Texture2D bulletTexture; 

static Bullet bulletPool[MAX_BULLETS];
static int currentAmmo;


void InitBulletPool(void) {

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletPool[i].active = false;
        bulletPool[i].radius = BULLET_RADIUS;
        bulletPool[i].color = YELLOW; 
    }
    
    bulletTexture = LoadTexture("assets/Sprites/BALA.png"); 
    currentAmmo = MAX_AMMO;
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


    if (currentAmmo > 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bulletPool[i].active) {
                bulletPool[i].active = true;
                bulletPool[i].position = startPos;
                bulletPool[i].speed.x = BULLET_SPEED * direction;
                bulletPool[i].speed.y = 0;

                currentAmmo--;
                return;
            }
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

bool CheckBulletCollision(Rectangle targetRect, int *damageTaken) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bulletPool[i].active) continue;

        float scaledWidth = bulletTexture.width * BULLET_SCALE;
        float scaledHeight = bulletTexture.height * BULLET_SCALE;
        Rectangle bulletRect = {
            bulletPool[i].position.x - (scaledWidth / 2.0f),
            bulletPool[i].position.y - (scaledHeight / 2.0f),
            scaledWidth,
            scaledHeight
        };

        if (CheckCollisionRecs(targetRect, bulletRect)) {
            bulletPool[i].active = false; 
            *damageTaken = BULLET_DAMAGE; 
            return true;
        }
    }
    
    *damageTaken = 0;
    return false; 
}

void UnloadBulletAssets(void) {
    UnloadTexture(bulletTexture);
}

int GetCurrentAmmo(void) {
    return currentAmmo;
}

void DrawAmmoCount(bool isReloading) {
    int posX = 20;
    int posY = 20 + 20 + 10;
    int fontSize = 20;

    if (isReloading) {
        DrawText("RECARREGANDO...", posX, posY, fontSize, ORANGE); 
    } else if (currentAmmo == 0) {
        DrawText("SEM MUNIÇÃO! (R)", posX, posY, fontSize, RED);
    } else {
        DrawText(TextFormat("Balas: %d / %d", GetCurrentAmmo(), MAX_AMMO), posX, posY, fontSize, WHITE);
    }
}

void ReloadAmmo(void) {
    currentAmmo = MAX_AMMO;
}



static Bullet enemyBulletPool[MAX_ENEMY_BULLETS];

void InitEnemyBulletPool(void) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBulletPool[i].active = false;
    }
}

void UnloadEnemyBulletAssets(void) {
    
}

void DespawnAllPlayerBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletPool[i].active = false;
    }
}

void DespawnAllEnemyBullets(void) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBulletPool[i].active = false;
    }
}

void SpawnEnemyBullet(Vector2 startPos, int direction, float speed, int damage) {
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!enemyBulletPool[i].active) {
            Bullet *bullet = &enemyBulletPool[i];
            
            bullet->active = true;
            bullet->position = startPos;
            bullet->damage = damage; 

            bullet->speed.x = speed * direction;
            bullet->speed.y = 0;
            
            return; 
        }
    }
}

void UpdateEnemyBulletPool(int screenWidth, int screenHeight) {
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        
        if (!enemyBulletPool[i].active) {
            continue;
        }

        Bullet *bullet = &enemyBulletPool[i];

        bullet->position.x += bullet->speed.x;
        bullet->position.y += bullet->speed.y;

        if (bullet->position.x < 0 || 
            bullet->position.x > screenWidth ||
            bullet->position.y < 0 || 
            bullet->position.y > screenHeight) 
        {
            bullet->active = false;
        }
    }
}

void DrawEnemyBulletPool(void) {
    extern Texture2D bulletTexture; 

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemyBulletPool[i].active) {          
            float scaledWidth = bulletTexture.width * BULLET_SCALE;
            float scaledHeight = bulletTexture.height * BULLET_SCALE;

            Vector2 pos = { 
                enemyBulletPool[i].position.x - (scaledWidth / 2.0f), 
                enemyBulletPool[i].position.y - (scaledHeight / 2.0f) 
            };

            DrawTextureEx(bulletTexture, pos, 0.0f, BULLET_SCALE, WHITE);
        }
    }
}


bool CheckEnemyBulletCollision(Rectangle targetRect, int *damageTaken) {
    
    extern Texture2D bulletTexture; 

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!enemyBulletPool[i].active) {
            continue;
        }

        Bullet *bullet = &enemyBulletPool[i]; 

        float scaledWidth = bulletTexture.width * BULLET_SCALE;
        float scaledHeight = bulletTexture.height * BULLET_SCALE;
        Rectangle bulletRect = {
            bullet->position.x - (scaledWidth / 2.0f),
            bullet->position.y - (scaledHeight / 2.0f),
            scaledWidth,
            scaledHeight
        };

        if (CheckCollisionRecs(targetRect, bulletRect)) {
            bullet->active = false;          
            *damageTaken = bullet->damage;   
            return true;
        }
    }
    
    *damageTaken = 0;
    return false; 
}