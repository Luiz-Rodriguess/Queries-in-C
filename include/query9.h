#ifndef query9
#define query9
#include "../include/table.h"
#include <stdio.h>

/* Estrutura auxiliar para armazenar as informações relevantes à query 9 */
typedef struct query9Aux Q9AUX;

/* Função que recebe uma linha com as informações referentes a query 9, separa os argumentos e incia a execução */
void formatQuery9(Tables* t,char* line,int format,FILE* output);

/* Função que cria uma Q9AUX com as informações indicadas */
Q9AUX* newQ9(char* userId, char* name);

#endif 