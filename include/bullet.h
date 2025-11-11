#ifndef BULLET_H_
#define BULLET_H_

#include "raylib.h"


#define MAX_BULLETS 50   
#define BULLET_SPEED 12.0f 
#define BULLET_RADIUS 5.0f 

typedef struct Bullet {
    Vector2 position;
    Vector2 speed;
    bool active; 
    float radius;
    Color color;
    int damage; 
} Bullet;

typedef struct AmmoPack {
    Vector2 position;
    bool active;
} AmmoPack;

void InitBulletPool(void);
void UpdateBulletPool(int screenWidth, int screenHeight);
void DrawBulletPool(void);
void SpawnBullet(Vector2 startPos, int direction);
void SpawnAmmoPack(Vector2 position);;
int GetCurrentAmmo(void);
bool IsReloading(void);
void DrawAmmoCount(void);
void DrawAmmoPack(void);
bool CheckBulletCollision(Rectangle targetRect, int *damageTaken);
AmmoPack* GetAmmoPack(void);
Texture2D GetAmmoPackTexture(void); 
void ReloadAmmo(void);
void UnloadBulletAssets(void);
void DespawnAllPlayerBullets(void);

#define MAX_ENEMY_BULLETS 100 
#define ENEMY_BULLET_SPEED_MOJO 8.0f
#define ENEMY_BULLET_SPEED_MARVIN 13.0f

void InitEnemyBulletPool(void);
void UpdateEnemyBulletPool(int screenWidth, int screenHeight);
void DrawEnemyBulletPool(void);
void DespawnAllEnemyBullets(void);

void SpawnEnemyBullet(Vector2 startPos, int direction, float speed, int damage);

bool CheckEnemyBulletCollision(Rectangle targetRect, int *damageTaken);

void UnloadEnemyBulletAssets(void);

#endif