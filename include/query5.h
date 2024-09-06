#ifndef query5
#define query5
#include "../include/table.h"
#include <stdio.h>

/* Estrutura auxiliar para armazenar as informações relevantes à query 5 */
typedef struct query5Aux Q5AUX;

/* Função que recebe uma linha com as informações referentes a query 5, separa os argumentos e incia a execução */
void formatQuery5(Tables* t, char* line, int format, FILE* output);

/* Função que cria uma nova Q5AUX com as informações necessárias */
Q5AUX* newQ5(char* airline,char* destination,char* id,char* planeModel,char* sDDate);

#endif