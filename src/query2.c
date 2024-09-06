#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/userAccess.h"
#include "../include/passengers.h"
#include "../include/util.h"
#include "../include/query2.h"
#include "../include/print.h"

struct query2Aux{
    char* type;
    char* id;
    char* date;
};

Q2AUX* newQ2(char* date,char* id,char* type,int flag){
    Q2AUX* q = (Q2AUX*) malloc(sizeof(struct query2Aux));
    q->date = date;
    q->id = strdup(id);
    q->type = NULL;
    if (flag) q->type = strdup(type);
    return q;
}

void destroyQ2Aux(gpointer data){
    Q2AUX* q = (Q2AUX*) data;
    if(q){
        if(q->date) free(q->date);
        if(q->id) free(q->id);
        if(q->type) free(q->type);
        free(q);
    }
}

gint compareQ2(gconstpointer a, gconstpointer b,gpointer userData){
    Q2AUX* qA = (Q2AUX*) a;
    Q2AUX* qB = (Q2AUX*) b;

    if (strcmp(qA->date,qB->date) == 0){
        return strcmp(qA->id,qB->id);
    }

    if (endAfterStartDateTime(qA->date,qB->date)) return -1;
    else return 1;
}

gboolean toStringQ2(gpointer key, gpointer value, gpointer userData){
    Q2AUX* q = (Q2AUX*) key;
    GQueue* queue = (GQueue*) userData;
    char* date = getDateFromDateTime(q->date);
    size_t len;    
    if(q->type != NULL){
        len = snprintf(NULL,0,"%s;%s;%s",q->id,date,q->type) + 1;
        char str[len];
        snprintf(str,len,"%s;%s;%s",q->id,date,q->type);
        g_queue_push_tail(queue,(gpointer) strdup(str));
    }else{
        len = snprintf(NULL,0,"%s;%s",q->id,date) + 1;
        char str[len];
        snprintf(str,len,"%s;%s",q->id,date);
        g_queue_push_tail(queue,(gpointer) strdup(str));
    }
    if (date) free(date);
    return 0;
}

gboolean toStringQ2F(gpointer key, gpointer value, gpointer userData){
    Q2AUX* q = (Q2AUX*) key;
    GQueue* queue = (GQueue*) userData;
    char* date = getDateFromDateTime(q->date);
    size_t len;
    int entryNumber = g_queue_get_length(queue) + 1;
    if(q->type != NULL){
        len = snprintf(NULL,0,"--- %u ---\nid: %s\ndate: %s\ntype: %s",entryNumber,q->id,date,q->type) + 1;
        char str[len];
        snprintf(str,len,"--- %u ---\nid: %s\ndate: %s\ntype: %s",entryNumber,q->id,date,q->type);
        g_queue_push_tail(queue,(gpointer) strdup(str));
    }else{
        len = snprintf(NULL,0,"--- %u ---\nid: %s\ndate: %s",entryNumber,q->id,date) + 1;
        char str[len];
        snprintf(str,len,"--- %u ---\nid: %s\ndate: %s",entryNumber,q->id,date);
        g_queue_push_tail(queue,(gpointer) strdup(str));
    }
    if (date) free(date);
    return 0;
}

void runQuery2(Tables* t, char* userId,char* opt, int format, FILE* output){
    
    GTree* tree = g_tree_new_full(compareQ2,NULL,destroyQ2Aux,NULL);
    int res = 0;

    if(isActiveUserCTG(getUserCTG(t),userId)){
        if(opt == NULL){    
            res = getFlightDatesCTG(getUserCTG(t),userId,t,1,tree);
            res = userReservationDatesCTG(getUserCTG(t),userId,t,1,tree);
        }
        else if(strcmp(opt,"flights") == 0){
            res = getFlightDatesCTG(getUserCTG(t),userId,t,0,tree);
        }
        else{
            res = userReservationDatesCTG(getUserCTG(t),userId,t,0,tree);
        }

        FAC* fac = newFAC(output,1,g_tree_nnodes(tree),format);

        if (res){
            if(format) printResult(fac,structTreeToStringQueue(tree,toStringQ2F));
            else printResult(fac,structTreeToStringQueue(tree,toStringQ2));
        }

        if (fac) destroyFAC(fac);
    }

    if (tree) g_tree_destroy(tree);
    if(userId) free(userId);
    if(opt) free(opt);
}

void formatQuery2(Tables* t, char* line, int format, FILE* output){
    char* userId;
    char* opt = NULL;
    char* token;
    int field = 0;
    while((token = strsep(&line," "))){
        switch (field){
        case 1: 
            userId = strdup(removeEnter(token));
            break;
        case 2:
            opt = strdup(removeEnter(token));
        }
        field++;
    }
    
    runQuery2(t,userId,opt,format,output);
}