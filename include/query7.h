#ifndef query7
#define query7
#include "../include/table.h"
#include <stdio.h>

/* Estrutura auxiliar para armazenar as informações relevantes à query 7 */
typedef struct query7Aux Q7AUX;

/* Função que recebe uma linha com as informações referentes a query 7, separa os argumentos e incia a execução */
void formatQuery7(Tables* t,char* line,int format,FILE* output);

/* Função que cria uma nova Q7AUX com o aeroporto indicado */
Q7AUX* newQ7(char* airport);

/* Função que insere a informação do delay mediano daquele aeroporto */
void setDelay(Q7AUX* q, size_t delay);

#endif 