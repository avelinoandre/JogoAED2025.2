#ifndef GEMINI_AI_H
#define GEMINI_AI_H

typedef struct DynamicEnemyStats {
    int health;
    int damage;
    float speed;
    float attackCooldown;
    float scale;
} DynamicEnemyStats;


/**
 * @brief
 */
void Gemini_Init(void);

/**
 * @brief 
 *
 * @param playerScore 
 * @param gameTime 
 * @return 
 */
DynamicEnemyStats Gemini_GetBalancedStats(int playerScore, float gameTime);

#endif