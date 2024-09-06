#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>
#include "../include/print.h"
#include "../include/util.h"

/* Função de print Do modo batch */
void printBatch(gpointer value, gpointer userData){
    char* line = (char*) value;
    FAC* fac = (FAC*) userData;
    if(getFACCount(fac) <= getFACMax(fac)) fprintf(getFACOutput(fac),"%s\n",line);
    if (getFACFormat(fac)){
        if (getFACCount(fac) < getFACMax(fac)) fprintf(getFACOutput(fac),"\n");
    }
    incFAC(fac);
}

/* Função de print do modo interativo */
void printInteractive(gpointer value, gpointer userData){
    char* line = (char*) value;
    FAC* fac = (FAC*) userData;
    if(getFACCount(fac) <= getFACMax(fac)){
        if(getFACFormat(fac)){
            printf("\033c");
            printf("page %d of %d\n",getFACCount(fac),getFACMax(fac));
            printf("%s\n",line);
            printf("\nPress Enter to continue");
            getchar();
        }else{
            if (getFACCount(fac) % 10 == 1) {
                int add = 1;
                if (getFACMax(fac) % 10 == 0) add = 0;
                printf("\033c");
                printf("page %d of %d\n", 1 + getFACCount(fac) / 10, add + getFACMax(fac) / 10);
            }
            printf("%s\n",line);
            if ((getFACCount(fac) % 10 == 0) || getFACCount(fac) == getFACMax(fac)){
                printf("\nPress Enter to continue");
                getchar();
            }
        }
    }
    incFAC(fac);
}

/* Função que liberta as strings na GQueue */
void freeCharQueue(gpointer data){ if(data) free(data);}

void printResult(FAC* fac, GQueue* queue){
    if (queue){
        if (getFACOutput(fac)) g_queue_foreach(queue,printBatch,fac);
        else{ 
            if (g_queue_is_empty(queue)) {
                printf("No results\n");
                printf("\nPress Enter to continue\n");
                getchar();
            }else g_queue_foreach(queue,printInteractive,fac);
        }
        g_queue_free_full(queue,freeCharQueue);
    }
}

GQueue* structTreeToStringQueue(GTree* structTree,treeToString function){
    GQueue* queue = g_queue_new();
    g_tree_foreach(structTree,function,queue);
    return queue;
}

GQueue* stringToQueue(gpointer queryStruct, toString function){
    GQueue* queue = g_queue_new();
    g_queue_push_head(queue,(gpointer) function(queryStruct));
    return queue;
}