#include "gemini_ai.h"
#include "enemy.h"
#include "globals.h"
#include <stdlib.h> 
#include <stdio.h>

// --- Variáveis de Controle da IA ---
static int totalInimigosParaSpawnar;
static int inimigosJaSpawnados;
static float delayInicialTimer; 
static float delayEntreSpawnsTimer;
static bool iaAtiva;

// --- Variáveis de SKILL (Nível 2) ---

// Modificador da velocidade de spawn (1.0 = normal)
static float spawnRateModificador; 

// Guarda o estado de saúde do jogador no início da cena
static int saudeJogadorCache; 

// --- Constantes da IA ---
#define IA_DELAY_INICIAL 2.0f
#define IA_DELAY_SPAWN_BASE 1.5f

static Vector2 GerarPosicaoSpawn() {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    float spawnX;
    
    if (rand() % 2 == 0) {
        spawnX = (float)(screenWidth + 50 + (rand() % 150));
    } else {
        spawnX = (float)(-50 - (rand() % 150));
    }

    float spawnY = RUA_LIMITE_SUPERIOR + (rand() % (int)(screenHeight - RUA_LIMITE_SUPERIOR - 150));
    return (Vector2){spawnX, spawnY};
}

static void SpawnarUmInimigo() {
    Vector2 pos = GerarPosicaoSpawn();
    int randomType;

    // --- REGRA DE SELEÇÃO (Nível 2) ---
    // A IA decide O QUE spawnar com base na saúde do jogador
    
    if (saudeJogadorCache < 50) {
        // Vida Baixa: Só spawna inimigos melee (tipo 0 e 1)
        printf("IA (Seleção): Vida baixa. Spawning inimigo melee.\n");
        randomType = rand() % 2; // Apenas GARNET ou LIMAO
    } else {
        // Vida Normal/Alta: Spawna qualquer tipo
        printf("IA (Seleção): Vida OK. Spawning tipo aleatório.\n");
        randomType = rand() % 4; // Qualquer um dos 4
    }
    
    SpawnEnemy((EnemyType)randomType, pos);

    printf("IA: Spawnei inimigo %d/%d em (%.0f, %.0f)\n", 
           inimigosJaSpawnados + 1, 
           totalInimigosParaSpawnar, 
           pos.x, pos.y);
}


// --- Funções Públicas (do .h) ---

// ATUALIZADO: Agora recebe o Player
void IA_IniciaCena(SceneNode* cena, Player* jogador) {
    if (cena == NULL || jogador == NULL) return;

    // IA pega o total de inimigos base do mapa
    int totalInimigosBase = cena->enemyCount;
    
    if (cena->isCleared) {
        totalInimigosBase = 0;
    }

    // --- LÓGICA DE SKILL (Nível 2) ---
    float saudePercent = (float)jogador->health / (float)jogador->maxHealth;
    saudeJogadorCache = jogador->health; // Salva para a Regra de Seleção

    // --- REGRA DE QUANTIDADE (Nível 2) ---
    if (saudePercent < 0.5f) { // Menos de 50% de vida
        printf("IA (Skill): Jogador com vida baixa (%.0f%%). Reduzindo dificuldade.\n", saudePercent*100);
        totalInimigosParaSpawnar = totalInimigosBase / 2;
        if (totalInimigosParaSpawnar == 0 && totalInimigosBase > 0) {
            totalInimigosParaSpawnar = 1; // Pelo menos 1
        }
    } else {
        printf("IA (Skill): Jogador com vida OK (%.0f%%). Dificuldade normal.\n", saudePercent*100);
        totalInimigosParaSpawnar = totalInimigosBase;
    }
    
    // --- REGRA DE VELOCIDADE (Nível 2) ---
    if (saudePercent < 0.5f) {
        // 50% mais devagar
        spawnRateModificador = 1.5f; 
        printf("IA (Velocidade): Spawns 50%% mais LENTOS.\n");
    } else if (jogador->health >= jogador->maxHealth) { // 100% de vida
        // 30% mais rápido
        spawnRateModificador = 0.7f; 
        printf("IA (Velocidade): Vida cheia. Spawns 30%% mais RÁPIDOS.\n");
    } else {
        // Velocidade normal
        spawnRateModificador = 1.0f; 
        printf("IA (Velocidade): Spawns em velocidade normal.\n");
    }
    // --- Fim da Lógica de Skill ---


    // Reseta as variáveis da IA
    inimigosJaSpawnados = 0;
    iaAtiva = true;

    if (totalInimigosParaSpawnar == 0) {
        iaAtiva = false;
        return;
    }
    
    delayInicialTimer = IA_DELAY_INICIAL;
    delayEntreSpawnsTimer = 0.0f;

    printf("IA: Cena iniciada. Preparando para spawnar %d inimigos (Base era %d).\n", 
           totalInimigosParaSpawnar, totalInimigosBase);
}

void IA_Update(float deltaTime) {
    if (!iaAtiva) return;

    if (inimigosJaSpawnados >= totalInimigosParaSpawnar) {
        iaAtiva = false;
        printf("IA: Spawn de %d inimigos concluído.\n", totalInimigosParaSpawnar);
        return;
    }

    if (delayInicialTimer > 0.0f) {
        delayInicialTimer -= deltaTime;
        return;
    }

    delayEntreSpawnsTimer -= deltaTime;

    if (delayEntreSpawnsTimer <= 0.0f) {
        SpawnarUmInimigo();
        inimigosJaSpawnados++;

        // ATUALIZADO: Usa o modificador de velocidade
        delayEntreSpawnsTimer = (IA_DELAY_SPAWN_BASE + (float)(rand() % 10) / 10.0f) * spawnRateModificador;
    }
}

int IA_GetTotalInimigos() {
    return totalInimigosParaSpawnar;
}

bool IA_EstaAtiva() {
    return iaAtiva;
}