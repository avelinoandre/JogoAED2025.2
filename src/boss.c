#include "boss.h"
#include "enemy.h"    
#include "globals.h"  
#include <stdio.h>
#include <stdlib.h>

static void SpawnMinions(int count) {
    printf("BOSS: Spawnando %d minions!\n", count);
    const int screenWidth = 1600;
    const int screenHeight = 900;

    for (int i = 0; i < count; i++) {
        float spawnX = (rand() % 2 == 0) ? -50.0f : (float)(screenWidth + 50);
        float spawnY = RUA_LIMITE_SUPERIOR + (rand() % (int)(screenHeight - RUA_LIMITE_SUPERIOR - 150));
        
        int randomType = 1 + (rand() % 3); 
        
        SpawnEnemy((EnemyType)randomType, (Vector2){spawnX, spawnY});
    }
}

void Boss_Spawn(DynamicEnemyStats stats) {
    
    Vector2 pos = (Vector2){1600 / 2.0f, RUA_LIMITE_SUPERIOR + 100.0f};
    
    Enemy* boss = SpawnEnemy(ENEMY_GARNET, pos);

    if (boss == NULL) {
        printf("ERRO: Falha ao spawnar o Boss! Pool de inimigos cheio?\n");
        return;
    }

    boss->health = stats.health;
    boss->maxHealth = stats.health;
    boss->speed = stats.speed;
    boss->damage = stats.damage;
    boss->attackCooldown = stats.attackCooldown;
    boss->scale = stats.scale;
    
    boss->isBoss = true;
    boss->spawnedAt50 = false;
    boss->spawnedAt25 = false;
    
    printf("BOSS SPAWNADO! Vida: %d, Dano: %d\n", boss->health, boss->damage);
}

void Boss_Update(Player* player) {
    Enemy* boss = NULL;
    int max = GetMaxEnemies();
    for (int i = 0; i < max; i++) {
        Enemy* e = GetEnemyFromPool(i);
        if (e && e->active && e->isBoss) {
            boss = e;
            break; 
        }
    }

    if (boss == NULL) {
        return;
    }

    float healthPercent = (float)boss->health / (float)boss->maxHealth;

    if (healthPercent <= 0.50f && !boss->spawnedAt50) {
        boss->spawnedAt50 = true; 
        SpawnMinions(2);
    }

    if (healthPercent <= 0.25f && !boss->spawnedAt25) {
        boss->spawnedAt25 = true; 
        SpawnMinions(2);
    }
}