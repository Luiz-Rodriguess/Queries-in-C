#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/util.h"
#include "../include/query6.h"
#include "../include/flightAccess.h"
#include "../include/print.h"

struct query6Aux{
    int numberOfPassengers;
    char* airport;
};

void destroyQ6(gpointer value){
    Q6AUX* q = (Q6AUX*) value;
    if(q){
        if(q->airport) free(q->airport);
        free(q);
    }
}

Q6AUX* newQ6(char* airport){
    Q6AUX* q = (Q6AUX*) malloc(sizeof(struct query6Aux));
    q->numberOfPassengers = 0;
    q->airport = strdup(airport);
    return q;
}

char* getAirportQ6(Q6AUX* q) { return q->airport;}

void incPassengers(Q6AUX* q, int numberOfPassengersToAdd) { q->numberOfPassengers += numberOfPassengersToAdd; }

gboolean toStringQ6(gpointer key, gpointer value ,gpointer userData){
    Q6AUX* q = (Q6AUX*) key;
    GQueue* queue = (GQueue*) userData;
    size_t len = snprintf(NULL,0,"%s;%d",q->airport,q->numberOfPassengers) + 1;
    char str[len];
    snprintf(str,len,"%s;%d",q->airport,q->numberOfPassengers);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gboolean toStringQ6F(gpointer key, gpointer value, gpointer userData){
    Q6AUX* q = (Q6AUX*) key;
    GQueue* queue = (GQueue*) userData;
    guint entryNumber = g_queue_get_length(queue)+1;
    size_t len = snprintf(NULL,0,"--- %d ---\nname: %s\npassengers: %d",entryNumber,q->airport,q->numberOfPassengers) + 1;
    char str[len];
    snprintf(str,len,"--- %d ---\nname: %s\npassengers: %d",entryNumber,q->airport,q->numberOfPassengers);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gint compareQ6(gconstpointer a, gconstpointer b, gpointer userData){
    Q6AUX* qA = (Q6AUX*) a;
    Q6AUX* qB = (Q6AUX*) b;

    if(qA->numberOfPassengers == qB->numberOfPassengers){
        if(strcmp(qA->airport,qB->airport) < 0) return -1;
        else return 1;
    }

    if(qA->numberOfPassengers > qB->numberOfPassengers) return -1;
    else return 1;
}

void runQuery6(Tables* t,char* year,int N, int format, FILE* output){
    
    GTree* tree = g_tree_new_full(compareQ6,NULL,destroyQ6,NULL);

    passengersOnYear(getFlightCTG(t),year,tree);
    int max = N;
    int len = g_tree_nnodes(tree);
    if (N >= len) max = len; 

    FAC* fac = newFAC(output,1,max,format);

    if (tree){
        if (format) printResult(fac,structTreeToStringQueue(tree,toStringQ6F));
        else printResult(fac,structTreeToStringQueue(tree,toStringQ6));
    }

    if (fac) destroyFAC(fac);
    if (year) free(year);
    if (tree) g_tree_destroy(tree);
}

void formatQuery6(Tables* t,char* line,int format,FILE* output){
    char* year;
    int N;
    char* token;
    int field = 0;
    while((token = strsep(&line," "))){
        switch (field){
        case 1:
            year = strdup(token);
            break;
        case 2:
            N = atoi(removeEnter(token));
        }
        field++;
    }

    runQuery6(t,year,N,format,output);
}