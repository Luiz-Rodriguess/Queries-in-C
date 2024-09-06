#ifndef query4
#define query4
#include "../include/table.h"

/* Função que recebe uma linha com as informações referentes a query 4, separa os argumentos e incia a execução */
typedef struct query4Aux Q4AUX;

/* Função que recebe uma linha com as informações referentes a query 4, separa os argumentos e incia a execução */
void formatQuery4(Tables* t,char* line,int format,FILE* output);

/* Função que cria uma nova Q4AUX com as informações necessárias */
Q4AUX* newQ4(char* resId,char* beginDate, char* endDate, char* userId, char* rating, double totalPrice);

#endif