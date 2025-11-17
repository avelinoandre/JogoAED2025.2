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

// Lógica 1P
#define MIN_INIMIGOS_POR_CENA_1P 3       
#define MAX_INIMIGOS_POR_CENA_1P 10      
#define MAX_INIMIGOS_SIMULTANEOS_1P 4   

// Lógica 2P
#define MIN_INIMIGOS_POR_CENA_2P 6       
#define MAX_INIMIGOS_POR_CENA_2P 14      
#define MAX_INIMIGOS_SIMULTANEOS_2P 5  


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
        
        spawnX = (float)(screenWidth + 50 + (rand() % 100));
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
        printf("ControleSpawn (Tático): Vida cheia. Enviando especialista (Spinel/Marvin).\n");
        grupoInimigo = 1 + (rand() % 2); 
    }

    switch (grupoInimigo) {
        case 0: 
            randomType = (rand() % 2 == 0) ? ENEMY_LIMAO_PRETO : ENEMY_LIMAO_BRANCO;
            break;
        case 1: 
            randomType = ENEMY_SPINEL; 
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
    float y = RUA_LIMITE_SUPERIOR + (rand() % (int)(screenHeight - RUA_LIMITE_SUPERIOR - 150)); 
    
    return (Vector2){x, y};
}

void ControleSpawn_IniciaCena(SceneNode* cena, Player* player1, Player* player2, bool isPlayer2Active) {
    if (cena == NULL || player1 == NULL) return;

    Caixa_DespawnAll();

    bool is2P = isPlayer2Active; 

    if (cena->id == 4) {
        Vector2 pos1 = GerarPosicaoCaixa();
        Caixa_Spawn_At(pos1);
        Vector2 pos2 = GerarPosicaoCaixa();
        while (Vector2Distance(pos1, pos2) < 100.0f) { pos2 = GerarPosicaoCaixa(); }
        Caixa_Spawn_At(pos2);
    }
    else if ( (cena->id == 2 || cena->id == 3) && sceneHasCaixa[cena->id] ) {
        Vector2 pos1 = GerarPosicaoCaixa();
        Caixa_Spawn_At(pos1);

        if (is2P) { 
            Vector2 pos2 = GerarPosicaoCaixa();
            while (Vector2Distance(pos1, pos2) < 100.0f) { pos2 = GerarPosicaoCaixa(); }
            Caixa_Spawn_At(pos2);
        }
    }
    else if (cena->id == 1 && sceneHasCaixa[cena->id]) {
        Vector2 pos = GerarPosicaoCaixa();
        Caixa_Spawn_At(pos);
    }
    

    if (cena->id == 5) { 
        printf("\n============================================\n");
        printf("INFO: Entrando no Mapa 5 (Boss). Calculando status...\n");

        int score = Score_GetScore();
        float tempo = Score_GetTimer();

        DynamicEnemyStats bossStats = Gemini_GetBalancedStats(score, tempo); 

        if (isPlayer2Active) {
            bossStats.health = (int)(bossStats.health * 1.25f);
            printf("INFO: Modo 2P detectado! Vida do Boss aumentada para %d\n", bossStats.health);
        }

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

    int totalInimigosBase;
    float saudePercent;
    
    if (isPlayer2Active) {
        totalInimigosBase = 4 + (int)(cena->id * 3); 
        printf("ControleSpawn (2P - Progressão): Cena ID %d. Calculando %d inimigos base.\n", cena->id, totalInimigosBase);
    
        if (cena->id == 3 || cena->id == 4) {
            totalInimigosBase -= 2;
            printf("ControleSpawn (2P - Ajuste): Reduzindo contagem em 2 para o mapa %d. Total agora: %d\n", cena->id, totalInimigosBase);
        }

        saudePercent = ((float)player1->health / (float)player1->maxHealth + (float)player2->health / (float)player2->maxHealth) / 2.0f;
        saudeJogadorCache = (player1->health + player2->health) / 2;
    } 
    else 
    {
        totalInimigosBase = 2 + (int)(cena->id * 2); 
        printf("ControleSpawn (1P - Progressão): Cena ID %d. Calculando %d inimigos base.\n", cena->id, totalInimigosBase);

        saudePercent = (float)player1->health / (float)player1->maxHealth;
        saudeJogadorCache = player1->health; 
    }

    if (cena->isCleared) {
        totalInimigosBase = 0;
    }

    int totalCalculado = totalInimigosBase;
    if (saudePercent < 0.5f) { 
        totalCalculado = totalInimigosBase / 2;
    } 

    if (cena->isCleared) {
        totalInimigosParaSpawnar = 0;
    } else {
        if (isPlayer2Active) {
            totalInimigosParaSpawnar = ClampInt(totalCalculado, MIN_INIMIGOS_POR_CENA_2P, MAX_INIMIGOS_POR_CENA_2P);
        } else {
            totalInimigosParaSpawnar = ClampInt(totalCalculado, MIN_INIMIGOS_POR_CENA_1P, MAX_INIMIGOS_POR_CENA_1P);
        }
    }
    

    printf("ControleSpawn (Resultado): %d inimigos para spawnar.\n", totalInimigosParaSpawnar);
    
    if (saudePercent < 0.5f) spawnRateModificador = 1.5f; 
    else if (saudePercent >= 1.0f) spawnRateModificador = 0.7f; 
    else spawnRateModificador = 1.0f; 

    inimigosJaSpawnados = 0;
    controleAtivo = true; 
    delayInicialTimer = DELAY_SPAWN_INICIAL;
    delayEntreSpawnsTimer = 0.0f;
}

void ControleSpawn_Update(float deltaTime, Player* player1, Player* player2, bool isPlayer2Active) {
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
        
        int maxSimultaneos = (isPlayer2Active) ? MAX_INIMIGOS_SIMULTANEOS_2P : MAX_INIMIGOS_SIMULTANEOS_1P;
        
        int budgetTela = maxSimultaneos - inimigosVivos;
        
        int restantesNaCena = totalInimigosParaSpawnar - inimigosJaSpawnados;

        int podeSpawnarAgora = budgetTela;
        if (restantesNaCena < podeSpawnarAgora) {
            podeSpawnarAgora = restantesNaCena;
        }

        if (podeSpawnarAgora > 0) {
            
            int quantosSpawnar = (rand() % podeSpawnarAgora) + 1;
            
            
            for (int i = 0; i < quantosSpawnar; i++) {
                if (inimigosJaSpawnados < totalInimigosParaSpawnar) { 
                    SpawnarUmInimigo(player1); 
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