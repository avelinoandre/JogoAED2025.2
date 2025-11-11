#ifndef ENEMY_H_
#define ENEMY_H_

#include "raylib.h"
#include "player.h" 

#define MAX_ENEMIES 10 

typedef enum {
    ENEMY_GARNET,  
    ENEMY_LIMAO,   
    ENEMY_MOJO,   
    ENEMY_MARVIN   
} EnemyType;

typedef struct Enemy {
    bool active;        
    EnemyType type;    
    Vector2 position;
    
    int health;
    int maxHealth;      
    float speed;        
    float scale;        
    int damage;         
    
    float attackTimer;
    float attackCooldown; 
    
    Texture2D *idleTextures;
    Texture2D *walkTextures;
    Texture2D *attackTextures; 

    int idleFrameCount;
    int walkFrameCount;
    int attackFrameCount;
    
    int currentFrame;
    int framesCounter;
    int framesSpeed; 
    
    bool isMoving;
    bool isAttacking;
    int direction; 

} Enemy;

void InitEnemyPool(void);

void UnloadEnemyAssets(void);

void UpdateEnemyPool(Player *player, int screenHeight);

void DrawEnemyPool(void);

void SpawnEnemy(EnemyType type, Vector2 position);

void DespawnAllEnemies(void); 

bool CheckBulletCollision(Rectangle enemyRect, int *damageTaken);

bool AreAllEnemiesDefeated(void);
int GetActiveEnemyCount(void);

#endif