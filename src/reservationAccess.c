#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/reservationAccess.h"
#include "../include/reservations.h"
#include "../include/util.h"
#include "../include/table.h"
#include "../include/query1.h"
#include "../include/query4.h"
#include "../include/query10.h"

struct reservationCTG{
    GHashTable* reservationTable;
    GHashTable* hotelReservations;
};

ReservationCTG* newReservationCTG(char* reservationCSV,gpointer userData){
    ReservationCTG* ctg = (ReservationCTG*) malloc(sizeof(struct reservationCTG));
    ReservationAux* aux = hashAtributeReservation(userData,reservationCSV);
    ctg->reservationTable = getReservationTable(aux);
    ctg->hotelReservations = getHotelReservations(aux);
    if(aux) free(aux);
    return ctg;
}

void destroyReservationCTG(ReservationCTG* ctg){
    if (ctg){
        if (ctg->reservationTable) destroyReservationTable(ctg->reservationTable);
        if (ctg->hotelReservations) g_hash_table_destroy(ctg->hotelReservations);
        free(ctg);
    }
}

double calculateUserTotalSpent(gpointer tables,GHashTable* userReservations){
    Tables* t = (Tables*) tables;
    ReservationCTG* ctg = (ReservationCTG*) getReservationCTG(t);
    return userResInfoQ1(ctg->reservationTable,userReservations);
}

gboolean existsReservation(ReservationCTG* ctg, char* reservationId){
    return g_hash_table_contains(ctg->reservationTable,reservationId);
}

Q1AUXR* reservationInfoQ1CTG(gpointer tables,char* reservationId){
    Tables* t = (Tables*) tables;
    ReservationCTG* ctg = getReservationCTG(t);
    Reservation* r = g_hash_table_lookup(ctg->reservationTable,reservationId);
    char* hoteId = getReservationHotelId(r);
    char* hotelName = getReservationHotelName(r);
    int hotelStars = getReservationHotelStars(r);
    char* beginDate = getReservationBeginDate(r);
    char* endDate = getReservationEndDate(r);
    char* includesBreakfast = getReservationIncludesBreakfast(r);
    int totalNights = getReservationNights(r);
    double totalPrice = getTotalPriceReservation(r,totalNights);
    return newQ1R(hoteId,hotelName,hotelStars,beginDate,endDate,includesBreakfast,totalNights,totalPrice);
}

char* getReservationBeginDateCTG(ReservationCTG* ctg, char* reservationId){
    Reservation* r = g_hash_table_lookup(ctg->reservationTable,reservationId);
    return getReservationBeginDate(r);
}

double getClassMedia(ReservationCTG* ctg,char* hotelId){
    GHashTable* reservationsFromId = g_hash_table_lookup(ctg->hotelReservations,hotelId);

    if (!reservationsFromId) return -1.0;
    
    double total = 0.0;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, reservationsFromId);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        char* reservationId = (char*) value;
        Reservation* r = (Reservation *) g_hash_table_lookup(ctg->reservationTable,reservationId);
        total += getReservationRating(r);
    }
    return total/g_hash_table_size(reservationsFromId);
}

int listHotelRes(ReservationCTG* ctg,char* hotelId,GTree* tree){

    GHashTable* reservationsFromId = g_hash_table_lookup(ctg->hotelReservations,hotelId);

    GHashTableIter iter;
    gpointer key, value;

    if (!reservationsFromId) return 0;

    g_hash_table_iter_init(&iter, reservationsFromId);
    while(g_hash_table_iter_next(&iter,&key,&value)){
        char* reservationId = (char*) value;
        Reservation * r = (Reservation *) g_hash_table_lookup(ctg->reservationTable,reservationId);
        
        char* id = getReservationId(r);
        char* beginDate = getReservationBeginDate(r);
        char* userId = getReservationUserId(r);
        char* endDate = getReservationEndDate(r);
        char* rating = getReservationRatingAsString(r);
        double totalPrice = getTotalPriceReservation(r,getReservationNights(r));

        Q4AUX* q = newQ4(id,beginDate,endDate,userId,rating,totalPrice);
        g_tree_insert(tree,q,NULL);
    }
    return 1;
}

int getRevenueHotel(ReservationCTG* ctg,char* hotelId,char* beginDate,char* endDate){
    GHashTable* reservationsFromId = g_hash_table_lookup(ctg->hotelReservations,hotelId);

    if (!reservationsFromId) return -1;
    
    int total = 0;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, reservationsFromId);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        char* reservationId = (char*) value;

        Reservation* r = (Reservation *) g_hash_table_lookup(ctg->reservationTable,reservationId);
        if(doesReservationIntersect(r,beginDate,endDate) == 1){
            int totalPriceForReservation = getTotalPriceQ8(r,beginDate,endDate);
            total += totalPriceForReservation;
        }
    }
    return total;
}

void numberOfReservations(ReservationCTG* ctg,GHashTable* tabelaQ10,char* yearOpt, char* monthOpt){
    int flag = 1;
    if(monthOpt) flag = 3; // Ano e mês como opção na query
    else if (yearOpt) flag = 2; // Ano como opção na query

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter,ctg->reservationTable);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        Reservation* r = (Reservation*) value;
        char* beginDate = getBeginDate(r);

        switch (flag){
            case 1 :{
                // caso em que não foi especificado nem ano nem mês
                char* year = getYearFromDate(beginDate);
                Q10AUX* q = g_hash_table_lookup(tabelaQ10,year);

                if(q == NULL){
                    q = newQ10(year);
                    g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                }
                
                incQ10Reservations(q);
                if(year) free(year);
                break;
            }
            case 2 : {
                // caso em que o ano é especificado
                if(isInOpt(beginDate,yearOpt)){
                    char* month = getMonthFromDate(beginDate);
                    Q10AUX* q = g_hash_table_lookup(tabelaQ10,month);

                    if(q == NULL){
                        q = newQ10(month);
                        g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                    }

                    incQ10Reservations(q);
                    if(month) free(month);
                }
                break;
            }
            case 3 :{
                // caso em que o ano e o mês são especificados 
                char opt [strlen(yearOpt) + strlen(monthOpt) + 2];
                sprintf(opt,"%s/%s",yearOpt,monthOpt);

                if(isInOpt(beginDate,opt)){
                    char* day = getDayFromDate(beginDate);
                    Q10AUX* q = g_hash_table_lookup(tabelaQ10,day);

                    if(q == NULL){
                        q = newQ10(day);
                        g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                    }

                    incQ10Reservations(q);
                    if(day) free(day);
                }
            }
        }
        if(beginDate) free(beginDate);
    }
}
