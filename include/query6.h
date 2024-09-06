#ifndef query6
#define query6
#include "../include/table.h"
#include <stdio.h>

/* Estrutura auxiliar para armazenar as informações relevantes à query 6 */
typedef struct query6Aux Q6AUX;

/* Função que recebe uma linha com as informações referentes a query 6, separa os argumentos e incia a execução */
void formatQuery6 (Tables* t,char* line,int format,FILE* output);

/* Função que cria uma nova Q6AUX com o aeroporto indicado*/
Q6AUX* newQ6(char* airport);

/* Função que aumenta o número de passageiros daquele aeroporto */
void incPassengers(Q6AUX* q, int numberOfPassengersToAdd);

/* Função que adquire o aeroporto utilizada para inserir as informações na tabela auxiliar */
char* getAirportQ6(Q6AUX* q);

#endif