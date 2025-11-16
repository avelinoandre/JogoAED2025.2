#include "gemini_ai.h"
#include "algoritmo_inimigos.h"
#include "enemy.h"    
#include "globals.h"
#include "player.h"   
#include "mapa.h"     
#include <stdlib.h> 
#include <stdio.h>
#include "score.h"
#include "boss.h"
#include "caixa.h" 
#include "raymath.h" 

#define DELAY_SPAWN_INICIAL 2.0f
#define DELAY_SPAWN_BASE 1.8f      
#define MIN_INIMIGOS_POR_CENA 3       
#define MAX_INIMIGOS_POR_CENA 10      
#define MAX_INIMIGOS_SIMULTANEOS 4

static int totalInimigosParaSpawnar;
static int inimigosJaSpawnados;
static float delayInicialTimer; 
static float delayEntreSpawnsTimer;
static bool controleAtivo;

static float spawnRateModificador; 
static int saudeJogadorCache; 

static int ClampInt(int valor, int minimo, int maximo) {
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

static Vector2 GerarPosicaoSpawn(Player* jogador) {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    float spawnX;

    float centroTela = screenWidth / 2.0f;
    int chanceSpawnDireita;

    if (jogador->position.x < centroTela - 200) {
       
        chanceSpawnDireita = 70; 
    } else if (jogador->position.x > centroTela + 200) {
        
        chanceSpawnDireita = 30; 
    } else {
        chanceSpawnDireita = 50; 
    }

    if (rand() % 100 < chanceSpawnDireita) {
        
        spawnX = (float)(screenWidth + 50 + (rand() % 150));
    } else {
        spawnX = (float)(-50 - (rand() % 150));
    }

    float spawnY = RUA_LIMITE_SUPERIOR + (rand() % (int)(screenHeight - RUA_LIMITE_SUPERIOR - 150));
    return (Vector2){spawnX, spawnY};
}

static void SpawnarUmInimigo(Player* jogador) {
    Vector2 pos = GerarPosicaoSpawn(jogador);

    float offsetX = (float)((rand() % 61) - 30); 
    float offsetY = (float)((rand() % 61) - 30);
    
    pos.x += offsetX;
    pos.y += offsetY;
        
    int randomType;
    
    int grupoInimigo = rand() % 3; 

    if (saudeJogadorCache < 50) {
        grupoInimigo = 0;
    } else if (jogador->health >= jogador->maxHealth && inimigosJaSpawnados == 0) {
        printf("ControleSpawn (Tático): Vida cheia. Enviando especialista (Mojo/Marvin).\n");
        grupoInimigo = 1 + (rand() % 2); 
    }

    switch (grupoInimigo) {
        case 0: 
            randomType = (rand() % 2 == 0) ? ENEMY_LIMAO_PRETO : ENEMY_LIMAO_BRANCO;
            break;
        case 1: 
            randomType = ENEMY_MOJO;
            break;
        case 2: 
            randomType = ENEMY_MARVIN;
            break;
    }

    SpawnEnemy((EnemyType)randomType, pos);

    printf("ControleSpawn: Spawnei inimigo %d/%d (Tipo: %d) em (%.0f, %.0f)\n", 
           inimigosJaSpawnados + 1, 
           totalInimigosParaSpawnar, 
           randomType,
           pos.x, pos.y);
}

static Vector2 GerarPosicaoCaixa(void) {
    int screenWidth = 1600;
    int screenHeight = 900;

    float x = (float)(rand() % (screenWidth - 600)) + 300; 
    float y = RUA_LIMITE_SUPERIOR + (rand() % (int)(screenHeight - RUA_LIMITE_SUPERIOR - 250)); 
    
    return (Vector2){x, y};
}


void ControleSpawn_IniciaCena(SceneNode* cena, Player* jogador) {
    if (cena == NULL || jogador == NULL) return;

    Caixa_DespawnAll();
    
    if (cena->id == 4) {
        Vector2 pos1 = GerarPosicaoCaixa();
        Caixa_Spawn_At(pos1);
        
        Vector2 pos2 = GerarPosicaoCaixa();
        while (Vector2Distance(pos1, pos2) < 100.0f) { 
             pos2 = GerarPosicaoCaixa();
        }
        Caixa_Spawn_At(pos2);
    }
    else if (cena->id > 0 && cena->id <= TOTAL_SCENES && sceneHasCaixa[cena->id]) {
        Vector2 pos = GerarPosicaoCaixa();
        Caixa_Spawn_At(pos);
    }

    if (cena->id == 5) { 
        printf("\n============================================\n");
        printf("INFO: Entrando no Mapa 5 (Boss). Calculando status...\n");

        int score = Score_GetScore();
        float tempo = Score_GetTimer();

        DynamicEnemyStats bossStats = Gemini_GetBalancedStats(score, tempo); 

        printf("--- Stats do Boss Calculados ---\n");
        printf("  Vida: %d\n", bossStats.health);
        printf("  Dano: %d\n", bossStats.damage);
        printf("  Velocidade: %.2f\n", bossStats.speed);
        printf("  Cooldown Atk: %.2fs\n", bossStats.attackCooldown);
        printf("  Scale (Tamanho): %.2f\n", bossStats.scale);
        printf("============================================\n\n");

        Boss_Spawn(bossStats);

        totalInimigosParaSpawnar = 1; 
        inimigosJaSpawnados = 1;      
        controleAtivo = false;       

        return; 
    }
    int totalInimigosBase = 2 + (int)(cena->id * 2);
    printf("ControleSpawn (Progressão): Cena ID %d. Calculando %d inimigos base.\n", cena->id, totalInimigosBase);

    if (cena->isCleared) {
        totalInimigosBase = 0;
    }

    float saudePercent = (float)jogador->health / (float)jogador->maxHealth;
    saudeJogadorCache = jogador->health; 
    int totalCalculado = totalInimigosBase;

    if (saudePercent < 0.5f) {
        totalCalculado = totalInimigosBase / 2;
    } 

    totalInimigosParaSpawnar = ClampInt(totalCalculado, MIN_INIMIGOS_POR_CENA, MAX_INIMIGOS_POR_CENA);
    if (cena->isCleared) {
        totalInimigosParaSpawnar = 0;
    }
    printf("ControleSpawn (Clamp): %d\n", totalInimigosParaSpawnar);

    
    if (saudePercent < 0.5f) spawnRateModificador = 1.5f; 
    else if (jogador->health >= jogador->maxHealth) spawnRateModificador = 0.7f; 
    else spawnRateModificador = 1.0f; 

    inimigosJaSpawnados = 0;
    controleAtivo = true; 
    delayInicialTimer = DELAY_SPAWN_INICIAL;
    delayEntreSpawnsTimer = 0.0f;
}

void ControleSpawn_Update(float deltaTime, Player* jogador) {
    if (!controleAtivo) return;

    if (inimigosJaSpawnados >= totalInimigosParaSpawnar) {
        if (controleAtivo) {
            controleAtivo = false;
            printf("ControleSpawn: Spawn de %d inimigos concluído.\n", totalInimigosParaSpawnar);
        }
        return;
    }

    if (delayInicialTimer > 0.0f) {
        delayInicialTimer -= deltaTime;
        return;
    }

    delayEntreSpawnsTimer -= deltaTime;

    if (delayEntreSpawnsTimer <= 0.0f) {
        
        int inimigosVivos = GetActiveEnemyCount();
        
        int budgetTela = MAX_INIMIGOS_SIMULTANEOS - inimigosVivos;
        
        int restantesNaCena = totalInimigosParaSpawnar - inimigosJaSpawnados;

        int podeSpawnarAgora = budgetTela;
        if (restantesNaCena < podeSpawnarAgora) {
            podeSpawnarAgora = restantesNaCena;
        }

        if (podeSpawnarAgora > 0) {
            
            int quantosSpawnar = (rand() % podeSpawnarAgora) + 1;
            
            
            for (int i = 0; i < quantosSpawnar; i++) {
                if (inimigosJaSpawnados < totalInimigosParaSpawnar) { 
                    SpawnarUmInimigo(jogador);
                    inimigosJaSpawnados++;
                }
            }

            float variacao = (float)(rand() % 10) / 10.0f;
            delayEntreSpawnsTimer = (DELAY_SPAWN_BASE + variacao) * spawnRateModificador;

        } 
        else if (inimigosVivos > 0 && restantesNaCena > 0) {
            delayEntreSpawnsTimer = 0.5f;
        }
        else {
            delayEntreSpawnsTimer = 1.0f; 
        }

    } 
    else {
        int inimigosVivos = GetActiveEnemyCount();
        if (inimigosVivos == 0 && controleAtivo) { 
            if (delayEntreSpawnsTimer > 0.3f) {
                delayEntreSpawnsTimer = 0.3f; 
            }
        }
    }
}

int ControleSpawn_GetTotalInimigos() {
    return totalInimigosParaSpawnar;
}

bool ControleSpawn_EstaAtivo() {
    return controleAtivo;
}

int ControleSpawn_GetInimigosSpawnados() {
    return inimigosJaSpawnados;
}