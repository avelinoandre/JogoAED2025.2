#include "bullet.h"
#include "raylib.h" 
#include "raymath.h"

#define BULLET_SCALE 4.0f
#define MAX_AMMO 10
#define BULLET_DAMAGE 25

static Texture2D bulletTexture; 

static Bullet bulletPool[MAX_BULLETS]; // Pool de balas do jogador
static int currentAmmo[2]; // [0] para P1, [1] para P2

static Sound shootSound;

// Prepara o "pool" de balas, carrega a textura e reseta a munição.
 
void InitBulletPool(void) {

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletPool[i].active = false;
        bulletPool[i].radius = BULLET_RADIUS;
        bulletPool[i].color = YELLOW; 
    }
    
    bulletTexture = LoadTexture("assets/Sprites/BALA.png"); 
    currentAmmo[0] = MAX_AMMO; 
    currentAmmo[1] = MAX_AMMO;

    shootSound = LoadSound("assets/audios/shootSound.wav"); 
    SetSoundVolume(shootSound, 0.4f);
}

// Desenha todas as balas ativas do jogador (Johnny) na tela.
 
void DrawBulletPool(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletPool[i].active) {
            
            float scaledWidth = bulletTexture.width * BULLET_SCALE;
            float scaledHeight = bulletTexture.height * BULLET_SCALE;

            Vector2 pos = { 
                bulletPool[i].position.x - (scaledWidth / 2.0f), 
                bulletPool[i].position.y - (scaledHeight / 2.0f) 
            };

            DrawTextureEx(bulletTexture, pos, 0.0f, BULLET_SCALE, WHITE);
        }
    }
}

/*
   Ativa uma bala do "pool" na posição do jogador, se houver munição.
   playerOwner 1 para P1, 2 para P2.
 */
void SpawnBullet(Vector2 startPos, int direction, int playerOwner) {

    int ownerIndex = playerOwner - 1; 

    if (currentAmmo[ownerIndex] > 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bulletPool[i].active) {
                bulletPool[i].active = true;
                bulletPool[i].position = startPos;
                bulletPool[i].speed.x = BULLET_SPEED * direction;
                bulletPool[i].speed.y = 0;
                currentAmmo[ownerIndex]--;

                PlaySound(shootSound);
                return;
            }
        }
    }
}

// Move todas as balas ativas do jogador e as desativa se saírem da tela.

void UpdateBulletPool(int screenWidth, int screenHeight) {

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletPool[i].active) {

            bulletPool[i].position.x += bulletPool[i].speed.x;
            bulletPool[i].position.y += bulletPool[i].speed.y;

            // Desativa a bala se sair da tela
            if (bulletPool[i].position.x < 0 || 
                bulletPool[i].position.x > screenWidth ||
                bulletPool[i].position.y < 0 ||
                bulletPool[i].position.y > screenHeight) 
            {
                bulletPool[i].active = false;
            }
        }
    }
}

/*
   Verifica se alguma bala ativa do jogador está colidindo com um retângulo (inimigo/caixa).
   Se colidir, a bala é desativada e o inimigo toma dano.
 */
bool CheckBulletCollision(Rectangle targetRect, int *damageTaken) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bulletPool[i].active) continue;

        float scaledWidth = bulletTexture.width * BULLET_SCALE;
        float scaledHeight = bulletTexture.height * BULLET_SCALE;
        Rectangle bulletRect = {
            bulletPool[i].position.x - (scaledWidth / 2.0f),
            bulletPool[i].position.y - (scaledHeight / 2.0f),
            scaledWidth,
            scaledHeight
        };

        if (CheckCollisionRecs(targetRect, bulletRect)) {
            bulletPool[i].active = false; 
            *damageTaken = BULLET_DAMAGE; 
            return true;
        }
    }
    
    *damageTaken = 0;
    return false; 
}

// Libera a textura e o som da bala da memória.

void UnloadBulletAssets(void) {
    UnloadTexture(bulletTexture);
    UnloadSound(shootSound);
}

// Retorna a quantidade de munição atual para um jogador específico.

int GetCurrentAmmo(int playerOwner) {
    return currentAmmo[playerOwner - 1];
}

// Desenha a interface de munição (contagem ou "RECARREGANDO...") na tela.

void DrawAmmoCount(bool isReloading, int playerOwner) {
    int barWidth = 200; 
    int posX;
    int posY = 20 + 20 + 10; // Posição Y (abaixo da barra de vida)
    int fontSize = 20;
    int ownerIndex = playerOwner - 1;

    if (playerOwner == 1) {
        posX = 20; 
    } else {
        posX = GetScreenWidth() - barWidth - 20;
    }


    if (isReloading) {
        DrawText("RECARREGANDO...", posX, posY, fontSize, ORANGE); 
    } else if (currentAmmo[ownerIndex] == 0) {
        // P1 usa 'R', P2 usa 'K'
        const char* reloadKey = (playerOwner == 1) ? "(R)" : "(K)";
        DrawText(TextFormat("SEM MUNIÇÃO! %s", reloadKey), posX, posY, fontSize, RED);
    } else {
        DrawText(TextFormat("BALAS: %d / %d", currentAmmo[ownerIndex], MAX_AMMO), posX, posY, fontSize, WHITE);
    }
}

// Define a munição de um jogador de volta ao máximo.

void ReloadAmmo(int playerOwner) {
    currentAmmo[playerOwner - 1] = MAX_AMMO;
}



// Funções das balas

static Bullet enemyBulletPool[MAX_ENEMY_BULLETS];

// Prepara o "pool" de balas dos inimigos.
 
void InitEnemyBulletPool(void) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBulletPool[i].active = false;
    }
}

// Desativa todas as balas do jogador (usado na troca de cena).

void DespawnAllPlayerBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletPool[i].active = false;
    }
}

// Desativa todas as balas dos inimigos (usado na troca de cena).

void DespawnAllEnemyBullets(void) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemyBulletPool[i].active = false;
    }
}

// Ativa uma bala do "pool" de inimigos (usado pelo Marvin e Mojo).
 
void SpawnEnemyBullet(Vector2 startPos, int direction, float speed, int damage) {
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!enemyBulletPool[i].active) {
            Bullet *bullet = &enemyBulletPool[i];
            
            bullet->active = true;
            bullet->position = startPos;
            bullet->damage = damage; 

            bullet->speed.x = speed * direction;
            bullet->speed.y = 0;
            
            return; 
        }
    }
}

// Move todas as balas ativas dos inimigos e as desativa se saírem da tela.
 
void UpdateEnemyBulletPool(int screenWidth, int screenHeight) {
    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        
        if (!enemyBulletPool[i].active) {
            continue;
        }

        Bullet *bullet = &enemyBulletPool[i];

        bullet->position.x += bullet->speed.x;
        bullet->position.y += bullet->speed.y;

        if (bullet->position.x < 0 || 
            bullet->position.x > screenWidth ||
            bullet->position.y < 0 || 
            bullet->position.y > screenHeight) 
        {
            bullet->active = false;
        }
    }
}

/*
   Desenha todas as balas ativas dos inimigos.
   (Reutiliza a textura da bala do jogador)
 */
void DrawEnemyBulletPool(void) {
    extern Texture2D bulletTexture; // Usa a textura da bala do P1

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemyBulletPool[i].active) {          
            float scaledWidth = bulletTexture.width * BULLET_SCALE;
            float scaledHeight = bulletTexture.height * BULLET_SCALE;

            Vector2 pos = { 
                enemyBulletPool[i].position.x - (scaledWidth / 2.0f), 
                enemyBulletPool[i].position.y - (scaledHeight / 2.0f) 
            };

            DrawTextureEx(bulletTexture, pos, 0.0f, BULLET_SCALE, WHITE);
        }
    }
}

// Verifica se alguma bala de inimigo está colidindo com um jogador.
 
bool CheckEnemyBulletCollision(Rectangle targetRect, int *damageTaken) {
    
    extern Texture2D bulletTexture; 

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!enemyBulletPool[i].active) {
            continue;
        }

        Bullet *bullet = &enemyBulletPool[i]; 

        float scaledWidth = bulletTexture.width * BULLET_SCALE;
        float scaledHeight = bulletTexture.height * BULLET_SCALE;
        Rectangle bulletRect = {
            bullet->position.x - (scaledWidth / 2.0f),
            bullet->position.y - (scaledHeight / 2.0f),
            scaledWidth,
            scaledHeight
        };

        if (CheckCollisionRecs(targetRect, bulletRect)) {
            bullet->active = false;          
            *damageTaken = bullet->damage;   
            return true;
        }
    }
    
    *damageTaken = 0;
    return false; 
}