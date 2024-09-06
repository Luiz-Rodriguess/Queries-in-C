#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/table.h"
#include "../include/util.h"
#include "../include/flightAccess.h"
#include "../include/statistics.h"
#include "../include/query5.h"
#include "../include/print.h"


struct query5Aux{
    char* id;
    char* sDDate;
    char* destination;
    char* airline;
    char* planeModel;
};

Q5AUX* newQ5(char* airline,char* destination,char* id,char* planeModel,char* sDDate){
    Q5AUX* q = (Q5AUX*) malloc(sizeof(struct query5Aux));
    q->airline = airline;
    q->destination = destination;
    q->id = id;
    q->planeModel = planeModel;
    q->sDDate = sDDate;
    return q;
}

void destroyQ5(gpointer value){
    Q5AUX* q = (Q5AUX*) value;
    if(q){
        if(q->airline) free(q->airline);
        if(q->destination) free(q->destination);
        if(q->id) free(q->id);
        if(q->planeModel) free(q->planeModel);
        if(q->sDDate) free(q->sDDate);
        free(q);
    }
}

gint compareQ5(gconstpointer a, gconstpointer b, gpointer userData){
    Q5AUX* qA = (Q5AUX*) a;
    Q5AUX* qB = (Q5AUX*) b;

    if (strcmp(qA->sDDate,qB->sDDate) == 0){
        if(atoi(qA->id) < atoi(qB->id)) return -1;
        else return 1;
    }
    if (endAfterStartDateTime(qA->sDDate,qB->sDDate)) return -1;
    else return 1;
}

gboolean toStringQ5(gpointer key, gpointer value, gpointer userData){
    Q5AUX* q = (Q5AUX*) key;
    GQueue* queue = (GQueue*) userData;
    size_t len = snprintf(NULL,0,"%s;%s;%s;%s;%s",q->id,q->sDDate,q->destination,q->airline,q->planeModel) + 1;
    char str[len];
    snprintf(str,len,"%s;%s;%s;%s;%s",q->id,q->sDDate,q->destination,q->airline,q->planeModel);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gboolean toStringQ5F(gpointer key, gpointer value, gpointer userData){
    Q5AUX* q = (Q5AUX*) key;
    GQueue* queue = (GQueue*) userData;
    guint entryNumber = g_queue_get_length(queue)+1;
    size_t len = snprintf(NULL,0,"--- %d ---\nid: %s\nschedule_departure_date: %s\ndestination: %s\nairline: %s\nplane_model: %s",entryNumber,q->id,q->sDDate,q->destination,q->airline,q->planeModel) + 1;
    char str[len];
    snprintf(str,len,"--- %d ---\nid: %s\nschedule_departure_date: %s\ndestination: %s\nairline: %s\nplane_model: %s",entryNumber,q->id,q->sDDate,q->destination,q->airline,q->planeModel);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

void runQuery5(Tables* t,char* airportName,char* beginDate, char* endDate, int format, FILE* output){
    GTree* tree = g_tree_new_full(compareQ5,NULL,destroyQ5,NULL);
    
    int res = listFLightsOrigin(getFlightCTG(t),airportName,beginDate,endDate,tree);

    FAC* fac = newFAC(output,1,g_tree_nnodes(tree),format);

    if (res){
        if(format) printResult(fac,structTreeToStringQueue(tree,toStringQ5F));
        else printResult(fac,structTreeToStringQueue(tree,toStringQ5));
    }
    
    if (fac) destroyFAC(fac);
    if (tree) g_tree_destroy(tree);
}

void formatQuery5(Tables* t, char* line, int format, FILE* output){
    
    char airportName[4];
    char beginDate[21];
    char endDate[21];
    int quotes = 0;
    line = removeEnter(line);

    // verificamos se as datas estão entre aspas
    if(line[7] == '"' || line[6] == '"') quotes = 1;

    // guarda a posição atual da string em que devemos inserir o novo valor
    int pos = 0;
    int len = strlen(line);
    
    // se tiver aspas devemos alterar a string
    if(quotes){
        for(int i=0; i<len; i++){
            // se encontrarmos aspas não alteramos a string
            if(line[i] != '"'){ 
                line[pos] = line[i];
                pos++;
            }
        }
        
        // deixamos os espaços restantes em branco
        while(pos<len){
            line[pos] = ' ';
            pos++;
        }
         // fazemos com que o último caractere seja um \0 para indicar o fim da string
        line[pos] = '\0';
    }

    // verificamos se está presente um 'F' na string e atribuímos os valores para cada buffer
    if(format){
        strncpy(airportName,line+3,3);
        airportName[3] = '\0';
        strncpy(beginDate,line+7,20);
        beginDate[20] = '\0';
        strncpy(endDate,line+27,20);
        endDate[20] = '\0';
    }
    else{
        strncpy(airportName,line+2,3);
        airportName[3] = '\0';
        strncpy(beginDate,line+6,20);
        beginDate[20] = '\0';
        strncpy(endDate,line+26,20);
        endDate[20] = '\0';
    }

    for(unsigned long i=0;i<strlen(airportName);i++){
        airportName[i] = toupper(airportName[i]);
    }

    runQuery5(t,airportName,beginDate,endDate,format,output);
}