#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/util.h"
#include "../include/userAccess.h"
#include "../include/reservationAccess.h"
#include "../include/flightAccess.h"
#include "../include/print.h"

typedef struct query10Aux{
    int newUsers;
    int numberOfFlights;
    int totalPassengers;
    int uniquePassengers;
    int numberOfReservations;
    char* ymd; // year || month || day
    char* type;
} Q10AUX;

Q10AUX* newQ10(char* ymd){
    Q10AUX* q = (Q10AUX*) malloc(sizeof(struct query10Aux));
    q->newUsers = 0;
    q->numberOfFlights = 0;
    q->totalPassengers = 0;
    q->uniquePassengers = 0;
    q->numberOfReservations = 0;
    q->ymd = strdup(ymd);
    q->type = NULL;
    return q;
}

char* getYMDQ10(Q10AUX* q) { return q->ymd;}
void incQ10Users(Q10AUX* q) { q->newUsers++;}
void incQ10Flights(Q10AUX* q) { q->numberOfFlights++;}
void incQ10TotalPassengers(Q10AUX* q,int n) { q->totalPassengers += n;}
void setQ10UniquePassengers(Q10AUX* q,int n) { q->uniquePassengers = n;}
void incQ10Reservations(Q10AUX* q) { q->numberOfReservations++;}
void setYMDQ10(Q10AUX* q,char* ymd){ q->ymd = strdup(ymd);}
void setTypeQ10(Q10AUX* q,char* type){ q->type = strdup(type);}


Q10AUX* cloneQ10(Q10AUX* q,char* opt){
    Q10AUX* newQ = newQ10(q->ymd);
    newQ->newUsers = q->newUsers;
    newQ->numberOfFlights = q->numberOfFlights;
    newQ->totalPassengers = q->totalPassengers;
    newQ->uniquePassengers = q->uniquePassengers;
    newQ->numberOfReservations = q->numberOfReservations;
    newQ->type = opt;
    return newQ;
}

void destroyQ10(gpointer value){
    Q10AUX* q = (Q10AUX*) value;
    if(q){
        if(q->ymd) free(q->ymd);
        if(q->type) free(q->type);
        free(q);
    }
}

gboolean toStringQ10(gpointer key, gpointer value, gpointer userData){
    Q10AUX* q = (Q10AUX*) key;
    GQueue* queue = (GQueue*) userData;
    size_t len = snprintf(NULL,0,"%d;%d;%d;%d;%d;%d",atoi(q->ymd),q->newUsers,q->numberOfFlights,q->totalPassengers,q->uniquePassengers,q->numberOfReservations) + 1;
    char str[len];
    snprintf(str,len,"%d;%d;%d;%d;%d;%d",atoi(q->ymd),q->newUsers,q->numberOfFlights,q->totalPassengers,q->uniquePassengers,q->numberOfReservations);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gboolean toStringQ10F(gpointer key, gpointer value, gpointer userData){
    Q10AUX* q = (Q10AUX*) key;
    GQueue* queue = (GQueue*) userData;
    guint entryNumber = g_queue_get_length(queue)+1;
    size_t len = snprintf(NULL,0,"--- %d ---\n%s: %d\nusers: %d\nflights: %d\npassengers: %d\nunique_passengers: %d\nreservations: %d",entryNumber,q->type,atoi(q->ymd),q->newUsers,q->numberOfFlights,q->totalPassengers,q->uniquePassengers,q->numberOfReservations) + 1;
    char str[len];
    snprintf(str,len,"--- %d ---\n%s: %d\nusers: %d\nflights: %d\npassengers: %d\nunique_passengers: %d\nreservations: %d",entryNumber,q->type,atoi(q->ymd),q->newUsers,q->numberOfFlights,q->totalPassengers,q->uniquePassengers,q->numberOfReservations);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gint compareQ10(gconstpointer a, gconstpointer b, gpointer userData){
    Q10AUX* qA = (Q10AUX*) a;
    Q10AUX* qB = (Q10AUX*) b;
    
    if(atoi(qA->ymd) < atoi(qB->ymd)) return -1;
    return 1;
}

// função que executa a query 10
void runQuery10(Tables* t,char* year,char* month,int format,FILE* output){
    
    GTree* tree = g_tree_new_full(compareQ10,NULL,destroyQ10,NULL);
    GHashTable* tabelaQ10 = g_hash_table_new(g_str_hash,g_str_equal);

    flightStatsQ10(getFlightCTG(t),tabelaQ10,year,month);
    newUsers(getUserCTG(t),tabelaQ10,year,month);
    numberOfReservations(getReservationCTG(t),tabelaQ10,year,month);

    GHashTableIter iter;
    gpointer key, value;

    char* opt = "year";
    if(month){
        opt = "day";
    } else if(year){
        opt = "month";
    }

    g_hash_table_iter_init(&iter,tabelaQ10);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        Q10AUX* q = (Q10AUX*) value;
        if (format) setTypeQ10(q,opt);
        g_tree_insert(tree,q,NULL);
    }

    g_hash_table_destroy(tabelaQ10);

    FAC* fac = newFAC(output,1,g_tree_nnodes(tree),format);

    if(tree){
        if(format) printResult(fac,structTreeToStringQueue(tree,toStringQ10F));
        else printResult(fac,structTreeToStringQueue(tree,toStringQ10));
    }

    if(year) free(year);
    if(month) free(month);
    if(fac) destroyFAC(fac);
    if(tree) g_tree_destroy(tree);
}

void formatQuery10(Tables* t,char* line,int format,FILE* output){
    char* year = NULL;
    char* month = NULL;
    char* token;
    int field = 0;
    while((token = strsep(&line," "))){
        switch (field){
        case 1:
            year = strdup(removeEnter(token));
            break;
        case 2:
            month = strdup(removeEnter(token));
            break;
        }
        field++;
    }

    runQuery10(t,year,month,format,output);
}