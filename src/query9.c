#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/util.h"
#include "../include/userAccess.h"
#include "../include/query9.h"
#include "../include/print.h"

struct query9Aux{
    char* userId;
    char* name;
};

Q9AUX* newQ9(char* userId, char* name){
    Q9AUX* q = malloc(sizeof(struct query9Aux));
    q->name = name;
    q->userId = userId;
    return q;
}

void destroyQ9(gpointer value){
    Q9AUX* q = (Q9AUX*) value;
    if(q){
        if(q->name) free(q->name);
        if(q->userId) free(q->userId);
        free(q);
    }
}

gboolean toStringQ9(gpointer key, gpointer value, gpointer userData){
    Q9AUX* q = (Q9AUX*) key;
    GQueue* queue = (GQueue*) userData;
    size_t len = snprintf(NULL,0,"%s;%s",q->userId,q->name) + 1;
    char str[len];
    snprintf(str,len,"%s;%s",q->userId,q->name);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gboolean toStringQ9F(gpointer key, gpointer value, gpointer userData){
    Q9AUX* q = (Q9AUX*) key;
    GQueue* queue = (GQueue*) userData;
    guint entryNumber = g_queue_get_length(queue)+1;
    size_t len = snprintf(NULL,0,"--- %d ---\nid: %s\nname: %s",entryNumber,q->userId,q->name) + 1;
    char str[len];
    snprintf(str,len,"--- %d ---\nid: %s\nname: %s",entryNumber,q->userId,q->name);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gint compareQ9(gconstpointer a, gconstpointer b, gpointer userData){
    Q9AUX* qA = (Q9AUX*) a;
    Q9AUX* qB = (Q9AUX*) b;

    char* normalNameA = normalizeString(qA->name);
    char* normalNameB = normalizeString(qB->name);

    int aux = strcasecmp(normalNameA, normalNameB);
    if (aux == 0) {
        aux = strcasecmp(qA->name,qB->name);

        if(aux == 0){

            char* normalIdA = normalizeString(qA->userId);
            char* normalIdB = normalizeString(qB->userId);
        
            aux = strcasecmp(normalIdA,normalIdB);

            if (aux == 0){aux = strcasecmp(qA->userId,qB->userId);}

            free(normalIdA);
            free(normalIdB);
        }
    }

    if(normalNameA) free(normalNameA);
    if(normalNameB) free(normalNameB);

    return aux;
}

void runQuery9(Tables* t,char* prefix, int format, FILE* output){
    GTree* tree = g_tree_new_full(compareQ9,NULL, destroyQ9, NULL);
    usersWithPrefix(getUserCTG(t),prefix,tree);

    FAC* fac = newFAC(output,1,g_tree_nnodes(tree),format);

    if(tree){
        if(format) printResult(fac,structTreeToStringQueue(tree,toStringQ9F));
        else printResult(fac,structTreeToStringQueue(tree,toStringQ9));
    }

    if(tree) g_tree_destroy(tree);
    if(fac) destroyFAC(fac);
}

void formatQuery9(Tables* t,char* line,int format,FILE* output){
    char* prefix = NULL;
    char* token;
    int field = 0;
    int quotes = 0;
    int len;

    if(format){
        if(line[3] == '"') quotes = 1;
        len = strlen(line) - 5;
    }
    else{
        if(line[2] == '"') quotes = 1;
        len = strlen(line) - 4; 
    }

    if(quotes){
        char aux[len];
        if(format){
            strncpy(aux,line+4,len);
        }
        else{
            strncpy(aux,line+3,len);
        }
        aux[len-1] = '\0';
        runQuery9(t,aux,format,output);
    }
    else{
        while((token = strsep(&line," "))){
            switch (field){
            case 1:
                prefix = strdup(removeEnter(token));
                break;
            }
            field++;
        }
    runQuery9(t,prefix,format,output);
    if (prefix) free(prefix);
    }
}