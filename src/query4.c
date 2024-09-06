#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/util.h"
#include "../include/query4.h"
#include "../include/reservationAccess.h"
#include "../include/statistics.h"
#include "../include/print.h"

struct query4Aux{
    char* resId;
    char* beginDate;
    char* endDate;
    char* userId;
    char* rating;
    double totalPrice;
};

Q4AUX* newQ4(char* resId,char* beginDate, char* endDate, char* userId, char* rating, double totalPrice){
    Q4AUX* q = (Q4AUX*) malloc(sizeof(struct query4Aux));
    q->resId = resId;
    q->beginDate = beginDate;
    q->endDate = endDate;
    q->userId = userId;
    q->rating = rating;
    q->totalPrice = totalPrice;
    return q;
}

void destroyQ4(gpointer value){
    Q4AUX* q = (Q4AUX*) value;
    if(q){
        if(q->resId) free(q->resId);
        if(q->beginDate) free(q->beginDate);
        if(q->endDate) free(q->endDate);
        if(q->userId) free(q->userId);
        if(q->rating) free(q->rating);
        free(q);
    }
}

gint compareQ4(gconstpointer a, gconstpointer b, gpointer userData){
    Q4AUX* qA = (Q4AUX*) a;
    Q4AUX* qB = (Q4AUX*) b;

    if(strcmp(qA->beginDate,qB->beginDate) == 0){
        return strcmp(qA->resId,qB->resId);
    }

    if(endAfterStartDate(qA->beginDate,qB->beginDate)) return -1;
    return 1;
}

gboolean toStringQ4(gpointer key, gpointer value, gpointer userData){
    Q4AUX* q = (Q4AUX*) key;
    GQueue* queue = (GQueue*) userData;
    size_t len = snprintf(NULL,0,"%s;%s;%s;%s;%s;%.3f",q->resId,q->beginDate,q->endDate,q->userId,q->rating,q->totalPrice) + 1;
    char str[len];
    snprintf(str,len,"%s;%s;%s;%s;%s;%.3f",q->resId,q->beginDate,q->endDate,q->userId,q->rating,q->totalPrice);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

gboolean toStringQ4F(gpointer key, gpointer value, gpointer userData){
    Q4AUX* q = (Q4AUX*) key;
    GQueue* queue = (GQueue*) userData;
    guint entryNumber = g_queue_get_length(queue)+1;
    size_t len = snprintf(NULL,0,"--- %d ---\nid: %s\nbegin_date: %s\nend_date: %s\nuser_id: %s\nrating: %s\ntotal_price: %.3f",entryNumber,q->resId,q->beginDate,q->endDate,q->userId,q->rating,q->totalPrice) + 1;
    char str[len];
    snprintf(str,len,"--- %d ---\nid: %s\nbegin_date: %s\nend_date: %s\nuser_id: %s\nrating: %s\ntotal_price: %.3f",entryNumber,q->resId,q->beginDate,q->endDate,q->userId,q->rating,q->totalPrice);
    g_queue_push_tail(queue,(gpointer) strdup(str));
    return 0;
}

void runQuery4(Tables* t,char* hotelId,int format,FILE* output){
    GTree* tree = g_tree_new_full(compareQ4,NULL,destroyQ4,NULL);

    listHotelRes(getReservationCTG(t),hotelId,tree);
    FAC* fac = newFAC(output,1,g_tree_nnodes(tree),format);
    if (tree){
        if(format) printResult(fac,structTreeToStringQueue(tree,toStringQ4F));
        else printResult(fac,structTreeToStringQueue(tree,toStringQ4));
    }

    if(hotelId) free(hotelId);
    if(fac) destroyFAC(fac);
    if(tree) g_tree_destroy(tree);
}

void formatQuery4(Tables* t,char* line,int format,FILE* output){
    char* hotelId;
    char* token;
    int field = 0;
    while((token = strsep(&line," "))){
        switch (field){
        case 1:
            hotelId = strdup(removeEnter(token));
            break;
        }
        field++;
    }
    runQuery4(t,hotelId,format,output);
}