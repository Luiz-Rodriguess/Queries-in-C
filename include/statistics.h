#ifndef statistics
#define statistics

/* Estrutura que guarda estatísticas gerais para a execução de programa */
typedef struct stats Stats;

/* Função que cria a estrutura Stats */
Stats* newStats();

/* Função que liberta a memória associada à uma Stats */
void destroyStats(Stats* stats);

/* Função que retorna a GTree que contém a informação da query 7 */
GTree* getQ7Info(Stats* stats);

/* Função que verifica se a query 7 já foi executada e sua informação já foi inserida nos Stats */
int hasQ7Info(Stats* stats);

/* Função que insere a informação da query 7 nos Stats */
void setQ7Info(Stats* stats, GTree* q7Info);

#endif