#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/parser.h"
#include "../include/table.h"

#define LINESIZE 2048

void parse(char* inputFileName, GHashTable* table, parseLine fillTable, FILE* errors,gpointer userData){
    
    FILE* inputFile = fopen(inputFileName, "r");

    if (!inputFile) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
    
    // buffer que guardará a informação de cada linha do ficheiro
    char line[LINESIZE]; 

    // lemos a primeira linha que contém a informação do que está contido em cada coluna, que não é relevante para a execução
    if(!fgets(line,LINESIZE,inputFile)){
        printf("ficheiro vazio\n");
        exit(EXIT_FAILURE);
    }

    // se o ficheiro de erros não for nulo escrevemos a primeira linha do ficheiro .csv
    if(errors != NULL){
        fputs(line,errors);
    }

    // lemos de linha a linha o ficheiro de entrada, até encontrarmos o EOF    
    while (fgets(line, LINESIZE, inputFile)){ 
        // chamamos a função de preencher a tabela
        fillTable(table,line,errors,userData);
    }

    //fechamos o ficheiro .csv
    fclose(inputFile);
}
