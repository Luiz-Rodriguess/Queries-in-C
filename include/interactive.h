#ifndef interactive
#define interactive
#include "../include/table.h"

/* Função utilizda para obter o caminho para o dataset no modo interativo
   caso não seja especificado nenhum dataset o padrão é o dataset normal */
char* getPathToDataset();

/* Função que lê do stdin as queries que desejam ser executadas */
void readInfo(Tables* t);

#endif