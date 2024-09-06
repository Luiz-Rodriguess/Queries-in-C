#ifndef parser
#define parser
#include <glib.h>
#include <stdio.h>
#include "../include/table.h"

/* Função de preenchimento de uma tabela */
typedef void (*parseLine) (GHashTable* table, char *line, FILE *errors, gpointer userData);

/* Função que lê os datasets e e chama a parseLine indicada */
void parse(char* inputFileName,GHashTable* table, parseLine fillTable, FILE* errors,gpointer userData);

#endif
