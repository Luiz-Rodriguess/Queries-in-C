#ifndef print
#define print
#include "../include/util.h"
#include <glib.h>

/*  Função utilizada para passar a informação contida em uma GTree para uma string com o resultado
    A função deve inserir os resultados em uma GQueue */
typedef gboolean (*treeToString) (gpointer key, gpointer value, gpointer userData);

/*  Função utilizda para passar a informação contida em uma estrutura auxiliar para um string
    A função deve inserir o resultado em uma Gqueue */
typedef char* (*toString) (gpointer data);

/* Função geral de print dos resultados das queries */
void printResult(FAC* fac,GQueue* queue);

/* Função que cria a GQueue necessária para fazer o print e chama a treeToString para cada entrada */
GQueue* structTreeToStringQueue(GTree* structTree,treeToString function);

/* função que cria a GQueue necessária para fazer o print e chama a toString para a estrutura */
GQueue* stringToQueue(gpointer queryStruct, toString function);

#endif