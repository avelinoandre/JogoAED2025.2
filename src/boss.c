#include "boss.h"
#include "enemy.h"    
#include "globals.h"  
#include <stdio.h>
#include <stdlib.h>

static void SpawnMinions(int count) {
    printf("BOSS: Spawnando %d minions!\n", count);

    for (int i = 0; i < count; i++) {
        
        //tlvz ajuste
        float spawnX = 1000.0f + (rand() % 50); 
        
        // tlvz ajuste
        float spawnY = 400.0f + (rand() % 40); 
        
        int randomType = 1 + (rand() % 3); 

        SpawnEnemy((EnemyType)randomType, (Vector2){spawnX, spawnY});
    }
}

void Boss_Spawn(DynamicEnemyStats stats) {
    
    // Posição do beco (área azul)
    Vector2 pos = (Vector2){1000.0f, 450.0f}; //tlvz ajuste
    
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