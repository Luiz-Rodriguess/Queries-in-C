#ifndef handlequery
#define handlequery
#include <stdio.h>
#include "../include/table.h"

/*  Função utilizada para criar os ficheiros de resposta ou não dependendo do modo
    input: caminho para o ficheiro no modo de execução batch , pode ser NULL (modo de execução interativo) 
    opt: campo utilizado para indicar qual o output desejado do módulo de testes */
void interpreter(Tables* t,char* input,int opt);

/* Função que redireciona as informações lidas para a query desejada */
void handle(Tables* t,char* line,FILE* result);

#endif