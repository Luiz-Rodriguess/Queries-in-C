#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/util.h"
#include "../include/flightAccess.h"
#include "../include/query7.h"
#include "../include/statistics.h"
#include "../include/print.h"

struct query7Aux{
    size_t delay;
    char* airport;
};

void setDelay(Q7AUX* q, size_t delay) { q->delay = delay;}

Q7AUX* newQ7(char* airport){
    Q7AUX* q = (Q7AUX*) malloc(sizeof(struct query7Aux));
    q->airport = strdup(airport);
    q->delay = 0;
    return q;
}

void destroyQ7(gpointer value){
    Q7AUX* q = (Q7AUX*) value;
    if(q){
        if(q->airport) free(q->airport);
        free(q);
    }
}

gboolean toStringQ7(gpointer key, gpointer value, gpointer userData){
    Q7AUX* q = (Q7AUX*) key;
    GQueue* queue = (GQueue*) userData;

    size_t len = snprintf(NULL,0,"%s;%lu",q->airport,q->delay) + 1;
    char str[len];
    snprintf(str,len,"%s;%lu",q->airport,q->delay);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gboolean toStringQ7F(gpointer key, gpointer value, gpointer userData){
    Q7AUX* q = (Q7AUX*) key;
    GQueue* queue = (GQueue*) userData;
    guint entryNumber = g_queue_get_length(queue)+1;
    size_t len = snprintf(NULL,0,"--- %d ---\nname: %s\nmedian: %lu",entryNumber,q->airport,q->delay) + 1;
    char str[len];
    snprintf(str,len,"--- %d ---\nname: %s\nmedian: %lu",entryNumber,q->airport,q->delay);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gint compareQ7(gconstpointer a, gconstpointer b, gpointer userData){
    Q7AUX* qA = (Q7AUX*) a;
    Q7AUX* qB = (Q7AUX*) b;

    if(qA->delay == qB->delay){
        return strcmp(qA->airport,qB->airport);
    }

    if(qA->delay > qB->delay) return -1;
    return 1;
}

void runQuery7(Tables* t,int N, int format, FILE* output){
    Stats* stats = getStats(t);

    if (!hasQ7Info(stats)){
        GTree* newTree = g_tree_new_full(compareQ7,NULL,destroyQ7,NULL);
        getMedianDelays(getFlightCTG(t),newTree);
        setQ7Info(stats,newTree);
    }

    GTree* tree = getQ7Info(getStats(t));

    int max = N;
    int len = g_tree_nnodes(tree);
    if (N >= len) max = len; 

    FAC* fac = newFAC(output,1,max,format);

    if (tree){
        if(format) printResult(fac,structTreeToStringQueue(tree,toStringQ7F));
        else printResult(fac,structTreeToStringQueue(tree,toStringQ7));
    }
    
    if(fac) destroyFAC(fac);
}

void formatQuery7(Tables* t,char* line,int format,FILE* output){
    int N;
    char* token;
    int field = 0;
    while((token = strsep(&line," "))){
        switch (field){
        case 1:
            N = atoi(removeEnter(token));
            break;
        }
        field++;
    }
    
    runQuery7(t,N,format,output);
}