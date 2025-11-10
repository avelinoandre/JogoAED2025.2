#include "bullet.h"
#include "raylib.h" 

#define BULLET_SCALE 4.0f
#define AMMO_PACK_SCALE 4.0f
#define MAX_AMMO 10
#define AMMO_PACK_FRAMES 7     
#define AMMO_PACK_FRAME_SPEED 0.25f
#define BULLET_DAMAGE 25

static Texture2D bulletTexture; 
static Texture2D ammoPackTexture;
static Texture2D ammoPackTextures[AMMO_PACK_FRAMES];
static Bullet bulletPool[MAX_BULLETS];
static AmmoPack ammoPack;

static int currentAmmo;
static bool isReloading;
static int ammoPackCurrentFrame = 0;
static float ammoPackFrameTimer = 0.0f;

#define PASSIVE_RELOAD_TIME 5.0f 
static float passiveReloadTimer = 0.0f;


void InitBulletPool(void) {

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletPool[i].active = false;
        bulletPool[i].radius = BULLET_RADIUS;
        bulletPool[i].color = YELLOW; 
    }
    
    bulletTexture = LoadTexture("assets/Sprites/BALA.png"); 
    ammoPackTextures[0] = LoadTexture("assets/Sprites/bullet/bullet_animation1.png");
    ammoPackTextures[1] = LoadTexture("assets/Sprites/bullet/bullet_animation2.png");
    ammoPackTextures[2] = LoadTexture("assets/Sprites/bullet/bullet_animation3.png");
    ammoPackTextures[3] = LoadTexture("assets/Sprites/bullet/bullet_animation4.png");
    ammoPackTextures[4] = LoadTexture("assets/Sprites/bullet/bullet_animation5.png");
    ammoPackTextures[5] = LoadTexture("assets/Sprites/bullet/bullet_animation6.png");
    ammoPackTextures[6] = LoadTexture("assets/Sprites/bullet/bullet_animation7.png");

    ammoPack.active = false;
    currentAmmo = MAX_AMMO;
    isReloading = false;
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
                }

                return;
            }
        }
    }
    else if (!isReloading) 
    {
        isReloading = true;
    }
}

void SpawnAmmoPack(Vector2 position) {
    if (!ammoPack.active) {
        ammoPack.active = true;
        ammoPack.position = position; 

        ammoPackCurrentFrame = 0;
        ammoPackFrameTimer = 0.0f;
    }
}

void UpdateBulletPool(int screenWidth, int screenHeight) {
    if (ammoPack.active) {
        ammoPackFrameTimer += GetFrameTime();
        
        if (ammoPackFrameTimer >= AMMO_PACK_FRAME_SPEED) {
            ammoPackFrameTimer = 0.0f; 
            ammoPackCurrentFrame++;  
            
            if (ammoPackCurrentFrame >= AMMO_PACK_FRAMES) {
                ammoPackCurrentFrame = 0;
            }
        }
    }

    if (isReloading && !ammoPack.active) {
        passiveReloadTimer += GetFrameTime(); 

        if (passiveReloadTimer >= PASSIVE_RELOAD_TIME) {
            AddPassiveAmmo(); 
            passiveReloadTimer = 0.0f; 
        }
    } else {
        passiveReloadTimer = 0.0f;
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
    for (int i = 0; i < AMMO_PACK_FRAMES; i++) {
        UnloadTexture(ammoPackTextures[i]);
    }
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
        DrawText("SEM MUNIÇÃO!", posX, posY, fontSize, RED);
    } else {
        DrawText(TextFormat("Balas: %d / %d", GetCurrentAmmo(), MAX_AMMO), posX, posY, fontSize, WHITE);
    }
}

void DrawAmmoPack(void) {
    if (ammoPack.active) {
        DrawTextureEx(ammoPackTextures[ammoPackCurrentFrame], ammoPack.position, 0.0f, AMMO_PACK_SCALE, WHITE);
    }
}

AmmoPack* GetAmmoPack(void) {
    return &ammoPack;
}

Texture2D GetAmmoPackTexture(void) {
    return ammoPackTextures[ammoPackCurrentFrame];
}

void ReloadAmmo(void) {
    currentAmmo = MAX_AMMO;
    isReloading = false;
    ammoPack.active = false;

    ammoPackCurrentFrame = 0;
    ammoPackFrameTimer = 0.0f;
}

