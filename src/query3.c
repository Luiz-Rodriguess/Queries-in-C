#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/reservationAccess.h"
#include "../include/util.h"
#include "../include/query3.h"
#include "../include/statistics.h"
#include "../include/print.h"

typedef struct query3Aux{
    double valorMedio;
}Q3AUX;

char* toStringQ3(gpointer data){
    Q3AUX* q = (Q3AUX*) data;
    size_t len = snprintf(NULL,0,"%.3f",q->valorMedio) + 1;
    char str[len];
    snprintf(str,len,"%.3f",q->valorMedio);
    return strdup(str);
}

char* toStringQ3F(gpointer data){
    Q3AUX* q = (Q3AUX*) data;
    size_t len = snprintf(NULL,0,"--- 1 ---\nrating: %.3f",q->valorMedio) + 1;
    char str[len];
    snprintf(str,len,"--- 1 ---\nrating: %.3f",q->valorMedio);
    return strdup(str);
}

void runQuery3(Tables* t,char* hotelId,int format,FILE* output){
    
    double valorMedio = getClassMedia(getReservationCTG(t),hotelId);
    
    Q3AUX* q = (Q3AUX*) malloc(sizeof(struct query3Aux));
    q->valorMedio = valorMedio;

    FAC* fac =newFAC(output,1,1,format);
    if (q->valorMedio >= 0.0){
        if (format) printResult(fac,stringToQueue(q,toStringQ3F));
        else printResult(fac,stringToQueue(q,toStringQ3));
    }

    if(q) free(q);
    if(fac) destroyFAC(fac);
    if(hotelId) free(hotelId);
}

void formatQuery3(Tables* t,char* line,int format,FILE* output){
    char* hotelId ;
    int field = 0;
    char* token;
    while((token = strsep(&line," "))){
        switch (field){
        case 1:
            hotelId = strdup(removeEnter(token));
            break;
        }
        field++;
    }
    
    runQuery3(t,hotelId,format,output);
}