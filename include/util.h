#ifndef util
#define util

#include <stdio.h>
#include <glib.h>

/* Função utilizada para remover o enter do fim de uma string */
char* removeEnter(char* line);

/* Função que verifica se uma data é válida */
int checkDate(char* date);

/* Função que verifica se um DateTime é válido  */
int checkDateTime(char* dateTime);

/*  Função que verifica se o fim está depois do início de uma Date
    Se devolver 1 o primeiro item está depois ou são iguais
    Se devolver 0 o primeiro item está antes */
int endAfterStartDate(char* end, char* start);

/*  Função que verifica se o fim está depois do início de um DateTime
    Se devolver 1 o primeiro item está depois ou são iguais
    Se devolver 0 o primeiro item está antes  */
int endAfterStartDateTime(char* end, char* start);

/* Estrutura que guarda as informações de um FILE* e outras necessárias para o print */
typedef struct fileAndCounter FAC;

/* Função que cria um novo FAC */
FAC* newFAC(FILE* output,int count,int max,int format);

/* Função que liberta a memória associada à um FAC */
void destroyFAC(FAC* f);

/* Função que aumenta o counter de um FAC */
void incFAC(FAC* f);

/* Função que retorna o output de um FAC */
FILE* getFACOutput(FAC* f);

/* Função que retorna o counter de um FAC */
int getFACCount(FAC* f);

/* Função que retorna o máximo de um FAC */
int getFACMax(FAC* f);

/* Função que retorna o formato de um FAC */
int getFACFormat(FAC* f);

/*  Função que retorna um ano à partir de uma data 
    Deve ser libertado depois */
char* getYearFromDate(char* date);

/*  Função que retorna um mês à partir de uma data 
    Deve ser libertado depois */
char* getMonthFromDate(char* date);

/*  Função que retorna um dia à partir de uma data 
    Deve ser libertado depois */
char* getDayFromDate(char* date);

/*  Função que retorna um ano à partir de um DateTime 
    Deve ser libertado depois */
char* getDateFromDateTime(char* dateTime);

/*  Função que adiciona a hora 00:00:00 à uma data 
    Deve ser libertado depois */
char* addTimeToDate(char* date);

/*  Função que verifica se a data está dentro da opção passada 
    A opção deve seguir a mesma estrutura da data */
int isInOpt(char* date,char* opt);

/* Função que verifica se a linha passada representa um inteiro */
int checkInt(char* line);

/* Função que vai substitui as strings indesejadas dentro de uma string maior */
void substituiSubstrings(char *str,char *subStr,char *substituicao);

/* Função utilizada para normalizar uma string */
char* normalizeString(char* str);

/* Função utilizada para libertar a memória de GHashTables que estão contidas em um GHashTable */
void destroyAux(gpointer value);

/* Função que altera a cor do print para vermelho */
void red();

/* Função que retorna a cor do print para o padrão */
void reset();

/* Função que altera a cor do print para verde */
void green();

/* Função que altera a cor do print para amarelo */
void yellow();

#endif