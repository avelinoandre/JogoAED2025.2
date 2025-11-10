#ifndef BULLET_H_
#define BULLET_H_

#include "raylib.h"


#define MAX_BULLETS 50   //Limite de balas na tela
#define BULLET_SPEED 12.0f // Velocidade da bala
#define BULLET_RADIUS 5.0f // Tamanho da bala 

typedef struct Bullet {
    Vector2 position;
    Vector2 speed;
    bool active; 
    float radius;
    Color color;
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
int GetCurrentAmmo();
bool IsReloading(void);
void DrawAmmoCount(void);
void DrawAmmoPack(void);
bool CheckBulletCollision(Rectangle targetRect, int *damageTaken);
AmmoPack* GetAmmoPack(void);
Texture2D GetAmmoPackTexture(void); 
void ReloadAmmo(void); 

void UnloadBulletAssets(void);

#endif