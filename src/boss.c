#include "boss.h"
#include "enemy.h"    
#include "globals.h"  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

static bool bossWasSpawned = false;
static bool bossIsDead = false;
static bool bossIsShielded = false; 

// Reinicia o estado do boss (chamado no início de cada jogo).
   
void Boss_Init(void) {
    bossWasSpawned = false;
    bossIsDead = false;
    bossIsShielded = false; 
}

// Spawna inimigos menores (minions) e ativa o escudo do boss.
 
static void SpawnMinions(int count) {
    printf("BOSS: Spawnando %d minions!\n", count);

    for (int i = 0; i < count; i++) {
        
        float spawnX = 1000.0f + (rand() % 50); 
        float spawnY = 400.0f + (rand() % 40); 
        int randomType = rand() % 4; // Escolhe entre os 4 inimigos comuns

        SpawnEnemy((EnemyType)randomType, (Vector2){spawnX, spawnY});
    }

    bossIsShielded = true;
    printf("BOSS: Escudo ATIVADO!\n");
}

/*
  Cria a entidade do Boss (Mojo) no mapa, aplicando os stats (vida, dano, etc.)
  recebidos da IA (Gemini).
 */
void Boss_Spawn(DynamicEnemyStats stats) {
    
    Vector2 pos = (Vector2){1000.0f, 450.0f}; 
    
    Enemy* boss = SpawnEnemy(ENEMY_MOJO, pos); 

    if (boss == NULL) {
        printf("ERRO: Falha ao spawnar o Boss! Pool de inimigos cheio?\n");
        return;
    }

    // Aplica os stats dinâmicos
    boss->health = stats.health;
    boss->maxHealth = stats.health;
    boss->speed = stats.speed;
    boss->damage = stats.damage;
    boss->attackCooldown = stats.attackCooldown;
    boss->scale = stats.scale;
    
    boss->isBoss = true;
    boss->spawnedAt50 = false;
    boss->spawnedAt25 = false;

    bossWasSpawned = true;
    bossIsDead = false;
    bossIsShielded = false; 
    
    printf("BOSS SPAWNADO! Vida: %d, Dano: %d\n", boss->health, boss->damage);
}

/*
   Atualiza a lógica do Boss a cada frame.
   Verifica se o boss morreu, gerencia o escudo (desativa se minions morrerem)
   e spama minions novos quando a vida chega a 50% e 25%.
 */
void Boss_Update(Player* player) {
    Enemy* boss = NULL;
    
    // Encontra a instância do boss no pool de inimigos
    if (bossWasSpawned && !bossIsDead) {
        int max = GetMaxEnemies();
        for (int i = 0; i < max; i++) {
            Enemy* e = GetEnemyFromPool(i);
            if (e && e->active && e->isBoss) {
                boss = e;
                break; 
            }
        }

        if (boss == NULL) {
            printf("BOSS FOI DERROTADO!\n");
            bossIsDead = true;
            return; 
        }

    } else {
        return;
    }

    // Gerencia o escudo
    if (bossIsShielded) {
        int activeEnemies = GetActiveEnemyCount(); 
        
        if (activeEnemies == 1) { // Se só o Boss estiver vivo
            bossIsShielded = false;
            printf("BOSS: Escudo DESATIVADO! Minions derrotados.\n");
        }
    }
    
    // Gerencia o spawn de minions baseado na vida
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

// Verifica para o 'jogo.c' se o boss foi derrotado.

bool Boss_IsDefeated(void) {
    return bossIsDead;
}

// Verifica se o escudo do boss está ativo (para tornar o boss invulnerável).
 
bool Boss_IsShielded(void) {
    return bossIsShielded;
}