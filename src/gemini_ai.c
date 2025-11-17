#include "gemini_ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"
#include "cJSON.h"
#include <math.h> 


static char apiKey[256] = {0};


static const char* GEMINI_URL_BASE = "https://generativelanguage.googleapis.com/v1beta/models/gemini-flash-latest:generateContent?key=";


typedef struct RespostaWeb {
    char *buffer;
    size_t tamanho;
} RespostaWeb;

/**
 * @brief 
 */
static size_t EscreverDadosCallback(void *dados, size_t tamanho, size_t nmemb, void *ponteiroUsuario) {
    size_t tamanhoReal = tamanho * nmemb;
    RespostaWeb *mem = (RespostaWeb *)ponteiroUsuario;

    char *ptr = (char*)realloc(mem->buffer, mem->tamanho + tamanhoReal + 1);
    if (ptr == NULL) {
        printf("ERRO: Falha ao alocar memoria (realloc) no callback da curl\n");
        return 0;
    }

    mem->buffer = ptr;
    memcpy(&(mem->buffer[mem->tamanho]), dados, tamanhoReal);
    mem->tamanho += tamanhoReal;
    mem->buffer[mem->tamanho] = '\0';

    return tamanhoReal;
}

/**
 * @brief 
 */
void Gemini_Init(void) {
    FILE* configFile = fopen("config.txt", "r");
    if (configFile == NULL) {
        printf("DEBUG: [Gemini_Init] ERRO: Nao foi possivel abrir o 'config.txt'.\n");
        return;
    }
    if (fgets(apiKey, sizeof(apiKey), configFile) != NULL) {
        apiKey[strcspn(apiKey, "\n")] = 0;
        if (apiKey[0] == 0) {
            printf("DEBUG: [Gemini_Init] 'config.txt' lido, mas esta VAZIO.\n");
        } else {
            printf("DEBUG: [Gemini_Init] Chave da API carregada com sucesso.\n");
        }
    } else {
        printf("DEBUG: [Gemini_Init] ERRO: Nao foi possivel ler do 'config.txt'.\n");
    }
    fclose(configFile);
}

/**
 * @brief 
 */
static char* BuildGeminiPayload(const char* prompt) {
    cJSON *root = cJSON_CreateObject();
    cJSON *contents = cJSON_CreateArray();
    cJSON *contentPart = cJSON_CreateObject();
    cJSON *parts = cJSON_CreateArray();
    cJSON *partText = cJSON_CreateObject();

    cJSON_AddItemToObject(partText, "text", cJSON_CreateString(prompt));
    cJSON_AddItemToArray(parts, partText);
    cJSON_AddItemToObject(contentPart, "parts", parts);
    cJSON_AddItemToArray(contents, contentPart);
    cJSON_AddItemToObject(root, "contents", contents);

    cJSON *genConfig = cJSON_CreateObject();
    cJSON_AddItemToObject(genConfig, "maxOutputTokens", cJSON_CreateNumber(2048));
    cJSON_AddItemToObject(root, "generationConfig", genConfig);
    
    char *jsonString = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return jsonString;
}

/**
 * @brief 
 */
static DynamicEnemyStats GetDefaultStats(void) {
    printf("AVISO: Falha na API Gemini. Usando stats padrao (Normal).\n");
    DynamicEnemyStats stats;
    stats.health = 500;
    stats.damage = 25;
    stats.speed = 1.9f;
    stats.attackCooldown = 1.5f;
    // MODIFICADO: Reduzido o tamanho padrão
    stats.scale = 5.5f; 
    return stats;
}

/**
 * @brief 
 */
static char* BuildPrompt_Boss(int score, float time) {
    const char *promptHeader = 
        "Voce eh um balanceador de dificuldade (Game Balancer) para um jogo beat 'em up.\n"
        "Sua tarefa eh ajustar os atributos do proximo inimigo (Boss) com base no desempenho do jogador.\n"
        "Responda APENAS com um objeto JSON contendo os seguintes atributos:\n"
        "{ \"vida\": int, \"dano\": int, \"velocidade\": float, \"cooldown\": float }\n\n"
        "Use esta tabela como guia (pode interpolar):\n"
        "Performance (Score/Tempo) * 1000 | Dificuldade   | Vida | Dano | Velocidade | Cooldown\n"
        "---------------------------------------------------------------------------------\n"
        "< 15000 (Fraco)                | Facil         | 350  | 20   | 1.7        | 1.7\n"
        "16000 - 30000 (Ok)             | Normal        | 500  | 25   | 1.9        | 1.5\n"
        "31000 - 45000 (Bom)           | Dificil       | 650  | 30   | 2.1        | 1.3\n"
        "> 45000 (Excelente)        | Muito Dificil | 800  | 35   | 2.3        | 1.1\n"
        "\n"
        "Dados do Jogador:\n";


    size_t promptLen = strlen(promptHeader) + 512;
    char* fullPrompt = (char*)malloc(promptLen);
    if (fullPrompt == NULL) return NULL;

    snprintf(fullPrompt, promptLen,
             "%s"
             "Score: %d\n"
             "Tempo: %.1f segundos\n\n"
             "Por favor, gere os atributos (vida, dano, velocidade, cooldown) para o Boss.",
             promptHeader, score, time);
             
    return fullPrompt;
}


/**
 * @brief 
 */
DynamicEnemyStats Gemini_GetBalancedStats(int playerScore, float gameTime) {
    
    if (apiKey[0] == 0) {
        printf("ERRO: API Key nao carregada.\n");
        return GetDefaultStats(); 
    }

    CURL *curl;
    CURLcode res;
    DynamicEnemyStats stats; 
    

    char* prompt = BuildPrompt_Boss(playerScore, gameTime); 
    char* payload = BuildGeminiPayload(prompt);
    free(prompt);
    
    if (payload == NULL) return GetDefaultStats();

    RespostaWeb resposta = {0};
    resposta.buffer = (char*)malloc(1);
    resposta.tamanho = 0;
    
    char fullUrl[512];
    snprintf(fullUrl, sizeof(fullUrl), "%s%s", GEMINI_URL_BASE, apiKey);

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, fullUrl);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, EscreverDadosCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resposta);
        
        printf("IA: Consultando API do Gemini...\n");
        res = curl_easy_perform(curl);
        printf("IA: Resposta recebida\n");
        
        printf("\nDEBUG: --- INICIO DA RESPOSTA CRUA DA API ---\n");
        printf("%s\n", resposta.buffer ? resposta.buffer : "Nenhuma resposta recebida no buffer.");
        printf("DEBUG: --- FIM DA RESPOSTA CRUA DA API ---\n\n");
        

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() falhou: %s\n", curl_easy_strerror(res));
            stats = GetDefaultStats();
        } else {
            cJSON *jsonResponse = cJSON_Parse(resposta.buffer);
            if (jsonResponse == NULL) {
                printf("ERRO: Nao foi possivel parsear o JSON da resposta.\n");
                printf("DEBUG: Resposta Recebida:\n%s\n", resposta.buffer);
                stats = GetDefaultStats();
            } else {
                cJSON *error = cJSON_GetObjectItem(jsonResponse, "error");
                if (error) {
                    cJSON *errorMessage = cJSON_GetObjectItem(error, "message");
                    printf("ERRO DA API: %s\n", errorMessage ? errorMessage->valuestring : "Erro desconhecido");
                    stats = GetDefaultStats();
                } else {
                    cJSON *candidates = cJSON_GetObjectItem(jsonResponse, "candidates");
                    cJSON *firstCandidate = cJSON_GetArrayItem(candidates, 0);
                    cJSON *content = cJSON_GetObjectItem(firstCandidate, "content");
                    cJSON *parts = cJSON_GetObjectItem(content, "parts");
                    cJSON *firstPart = cJSON_GetArrayItem(parts, 0);
                    cJSON *text = cJSON_GetObjectItem(firstPart, "text");

                    if (text && cJSON_IsString(text)) {
                        char* strInicio = strstr(text->valuestring, "{");
                        char* strFim = strrchr(text->valuestring, '}');
                        char* strJsonLimpo = text->valuestring;
                        if (strInicio && strFim && strFim > strInicio) {
                            *(strFim + 1) = '\0';
                            strJsonLimpo = strInicio;
                        }

                        cJSON *innerJson = cJSON_Parse(strJsonLimpo);
                        if (innerJson) {
                            cJSON *health = cJSON_GetObjectItem(innerJson, "vida");
                            cJSON *damage = cJSON_GetObjectItem(innerJson, "dano");
                            cJSON *speed = cJSON_GetObjectItem(innerJson, "velocidade");
                            cJSON *cooldown = cJSON_GetObjectItem(innerJson, "cooldown");

                            if (health && damage && speed && cooldown) {
                                stats.health = health->valueint;
                                stats.damage = damage->valueint;
                                stats.speed = (float)speed->valuedouble;
                                stats.attackCooldown = (float)cooldown->valuedouble;

                                // *** LÓGICA DE ESCALA MODIFICADA ***
                                float healthBonus = (stats.health / 100.0f) * 0.4f;
                                float speedPenalty = (stats.speed / 2.0f) * 0.4f;
                                // Reduzido o mínimo de 6.0f para 5.0f e a base de 3.5f para 3.0f
                                stats.scale = fmaxf(5.0f, 3.0f + healthBonus - speedPenalty); 

                                printf("INFO: Stats dinâmicos recebidos da API!\n");
                            } else {
                                printf("ERRO: Nao achei as chaves (vida, dano, etc.) no JSON interno.\n");
                                printf("DEBUG: JSON Limpo: %s\n", strJsonLimpo);
                                stats = GetDefaultStats();
                            }
                            cJSON_Delete(innerJson);
                        } else {
                            printf("ERRO: Nao foi possivel parsear o JSON interno (limpo).\n");
                            printf("DEBUG: JSON Limpo: %s\n", strJsonLimpo);
                            stats = GetDefaultStats();
                        }
                    } else {
                        printf("ERRO: Nao foi possivel encontrar 'text' na resposta da API.\n");
                        stats = GetDefaultStats();
                    }
                }
                cJSON_Delete(jsonResponse);
            }
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        free(payload);
        free(resposta.buffer);
    } else {
        stats = GetDefaultStats();
    }

    return stats; 
}