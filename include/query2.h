#ifndef query2
#define query2
#include "../include/table.h"
#include <stdio.h>

/* Estrutura auxiliar para armazenar as informações relevantes à query 2*/
typedef struct query2Aux Q2AUX;

/* Função que recebe uma linha com as informações referentes a query 2, separa os argumentos e incia a execução */
void formatQuery2(Tables* t, char* line, int format, FILE* output);

/*  Função que cria uma nova instância de Q2AUX
    type: indica se a informação é de uma Flight ou de Reservation
    flag: indica se a informação é apenas a uma estrutura ou se é das duas, pode ser 0 ou 1*/
Q2AUX* newQ2(char* date,char* id,char* type,int flag);

#endif