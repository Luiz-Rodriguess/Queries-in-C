#ifndef query10
#define query10
#include "../include/table.h"

/* Estrutura auxiliar para armazenar as informações relevantes à query 10 */
typedef struct query10Aux Q10AUX;

/* Função que recebe uma linha com as informações referentes a query 10, separa os argumentos e incia a execução */
void formatQuery10(Tables* t,char* line,int format,FILE* output);

/*  Função que devolve o ymd para ser inserido na tabela
    ymd -> year | month | day */
char* getYMDQ10(Q10AUX* q);

/* Função que cria uma Q10AUX com as informações indicadas */
Q10AUX* newQ10(char* ymd);

/* Função que aumenta o número de novos users */
void incQ10Users(Q10AUX* q);

/* Função que aumenta o número de flights */
void incQ10Flights(Q10AUX* q);

/* Função que aumenta o número total de passageiros */
void incQ10TotalPassengers(Q10AUX* q,int n);

/* Função que altera o número de passageiros distintos */
void setQ10UniquePassengers(Q10AUX* q,int n);

/* Função que aumenta o número de reservas feitas */
void incQ10Reservations(Q10AUX* q);

#endif