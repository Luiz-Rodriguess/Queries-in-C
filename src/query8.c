#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/util.h"
#include "../include/reservationAccess.h"
#include "../include/statistics.h"
#include "../include/print.h"

typedef struct query8Aux{
    int totalRevenue;
}Q8AUX;

char* toStringQ8(gpointer data){
    Q8AUX* q = (Q8AUX*) data;
    size_t len = snprintf(NULL,0,"%d",q->totalRevenue) + 1;
    char str[len];
    snprintf(str,len,"%d",q->totalRevenue);
    return strdup(str);
}

char* toStringQ8F(gpointer data){
    Q8AUX* q = (Q8AUX*) data;
    size_t len = snprintf(NULL,0,"--- 1 ---\nrevenue: %d",q->totalRevenue) + 1;
    char str[len];
    snprintf(str,len,"--- 1 ---\nrevenue: %d",q->totalRevenue);
    return strdup(str);
}

void runQuery8(Tables* t,char* hotelId, char* beginDate, char* endDate, int format, FILE* output){
    int totalRevenue = getRevenueHotel(getReservationCTG(t),hotelId,beginDate,endDate);

    FAC* fac = newFAC(output,1,1,format);

    Q8AUX* q = (Q8AUX*) malloc(sizeof(struct query8Aux));
    q->totalRevenue = totalRevenue;
    if (q->totalRevenue != -1){
        if(format) printResult(fac,stringToQueue(q,toStringQ8F));
        else printResult(fac,stringToQueue(q,toStringQ8));
    }
    
    if (q) free(q);
    if (fac) destroyFAC(fac);
    if(hotelId) free(hotelId);
    if(beginDate) free(beginDate);
    if(endDate) free(endDate);
}

void formatQuery8(Tables* t,char* line,int format,FILE* output){
    char* hotelId;
    char* beginDate;
    char* endDate;
    int field = 0;
    char* token;
    while((token = strsep(&line," "))){
        switch (field){
        case 1:
            hotelId = strdup(token);
            break;
        case 2:
            beginDate = strdup(token);
            break;
        case 3:
            endDate = strdup(removeEnter(token));
            break;
        }
        field++;
    }
    
    runQuery8(t,hotelId,beginDate,endDate,format,output);
}