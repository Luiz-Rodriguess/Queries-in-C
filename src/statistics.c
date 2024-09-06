#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/statistics.h"

struct stats{
    GTree* q7Info;
};

Stats* newStats(){
    Stats* new = (Stats*) malloc(sizeof(struct stats));
    new->q7Info = NULL;
    return new;
}

void destroyStats(Stats* stats){
    if(stats){
        if (stats->q7Info) g_tree_destroy(stats->q7Info);
        free(stats);
    }
}

GTree* getQ7Info(Stats* stats){ return stats->q7Info;}

int hasQ7Info(Stats* stats) { return stats->q7Info != NULL ? 1 : 0;}

void setQ7Info(Stats* stats, GTree* q7Info){ stats->q7Info = q7Info;}