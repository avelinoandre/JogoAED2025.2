#include "gemini_ai.h"
#include "algoritmo_inimigos.h"
#include "enemy.h"    
#include "globals.h"
#include "player.h"   
#include "mapa.h"     
#include <stdlib.h> 
#include <stdio.h>


#define IA_DELAY_INICIAL 2.0f
#define IA_DELAY_SPAWN_BASE 1.8f      
#define IA_MIN_INIMIGOS_CENA 3       
#define IA_MAX_INIMIGOS_CENA 10      
#define IA_MAX_INIMIGOS_CONCORRENTES 4 
#define IA_CHANCE_SPAWN_DUPLO 30       
#define IA_DELAY_SPAWN_DUPLO 0.5f      

static int totalInimigosParaSpawnar;
static int inimigosJaSpawnados;
static float delayInicialTimer; 
static float delayEntreSpawnsTimer;
static bool iaAtiva;

static float spawnRateModificador; 
static int saudeJogadorCache; 

static int Clamp(int valor, int minimo, int maximo) {
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

    if (saudeJogadorCache < 50) {
        randomType = rand() % 2; 
    } 
    else if (jogador->health >= jogador->maxHealth && inimigosJaSpawnados == 0) {
        
        printf("IA (Tático): Vida cheia. Enviando especialista (Mojo/Marvin).\n");
        randomType = 2 + (rand() % 2); 
    }
    else {
        randomType = rand() % 4; 
    }
    
    SpawnEnemy((EnemyType)randomType, pos);

    printf("IA: Spawnei inimigo %d/%d em (%.0f, %.0f)\n", 
           inimigosJaSpawnados + 1, 
           totalInimigosParaSpawnar, 
           pos.x, pos.y);
}



void IA_IniciaCena(SceneNode* cena, Player* jogador) {
    if (cena == NULL || jogador == NULL) return;

    if (cena->id == 5) {
        printf("\n============================================\n");
        printf("INFO: Entrando no Mapa 5. Consultando API para Stats do Boss...\n");
        
        int score = Score_GetScore();
        float tempo = Score_GetTimer();
        
        DynamicEnemyStats bossStats = Gemini_GetBalancedStats(score, tempo); 
        
        printf("--- RESPOSTA DA API (Stats do Boss) ---\n");
        printf("  Vida: %d\n", bossStats.health);
        printf("  Dano: %d\n", bossStats.damage);
        printf("  Velocidade: %.2f\n", bossStats.speed);
        printf("  Cooldown Atk: %.2fs\n", bossStats.attackCooldown);
        printf("  Scale (Tamanho): %.2f\n", bossStats.scale);
        printf("============================================\n\n");
    }

    int totalInimigosBase = 2 + (int)(cena->id * 2);
    printf("IA (Progressão): Cena ID %d. Calculando %d inimigos base.\n", cena->id, totalInimigosBase);

    if (cena->isCleared) {
        totalInimigosBase = 0;
    }

    float saudePercent = (float)jogador->health / (float)jogador->maxHealth;
    saudeJogadorCache = jogador->health; 
    int totalCalculado = totalInimigosBase;

    if (saudePercent < 0.5f) {
        totalCalculado = totalInimigosBase / 2;
    } 
    
    totalInimigosParaSpawnar = Clamp(totalCalculado, IA_MIN_INIMIGOS_CENA, IA_MAX_INIMIGOS_CENA);
    if (cena->isCleared) {
        totalInimigosParaSpawnar = 0;
    }
    printf("IA (Clamp): Total final: %d\n", totalInimigosParaSpawnar);

    
    if (saudePercent < 0.5f) spawnRateModificador = 1.5f; 
    else if (jogador->health >= jogador->maxHealth) spawnRateModificador = 0.7f; 
    else spawnRateModificador = 1.0f; 

    inimigosJaSpawnados = 0;
    iaAtiva = true; 
    delayInicialTimer = IA_DELAY_INICIAL;
    delayEntreSpawnsTimer = 0.0f;
}

void IA_Update(float deltaTime, Player* jogador) {
    if (!iaAtiva) return;

    if (inimigosJaSpawnados >= totalInimigosParaSpawnar) {
        if (iaAtiva) {
            iaAtiva = false;
            printf("IA: Spawn de %d inimigos concluído.\n", totalInimigosParaSpawnar);
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
        
        int budgetTela = IA_MAX_INIMIGOS_CONCORRENTES - inimigosVivos;
        
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
            delayEntreSpawnsTimer = (IA_DELAY_SPAWN_BASE + variacao) * spawnRateModificador;

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
        if (inimigosVivos == 0 && iaAtiva) { 
            if (delayEntreSpawnsTimer > 0.3f) {
                delayEntreSpawnsTimer = 0.3f; 
            }
        }
    }
}

int IA_GetTotalInimigos() {
    return totalInimigosParaSpawnar;
}

bool IA_EstaAtiva() {
    return iaAtiva;
}

int IA_GetEnemiesSpawned() {
    return inimigosJaSpawnados;
}