#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/userAccess.h"
#include "../include/flightAccess.h"
#include "../include/reservationAccess.h"
#include "../include/util.h"
#include "../include/query1.h"
#include "../include/print.h"

struct query1AuxUser{
    char* name;
    char sex;
    int userAge;
    char*  countryCode;
    char* passport;
    int numberOfFlights;
    int totalRes;
    double totalPrice;
};

char* toStringQ1UserF(gpointer data){
    Q1AUXU* q = (Q1AUXU*) data;
    size_t len = snprintf(NULL,0,"--- 1 ---\nname: %s\nsex: %c\nage: %d\ncountry_code: %s\npassport: %s\nnumber_of_flights: %d\nnumber_of_reservations: %d\ntotal_spent: %.3f",q->name,q->sex,q->userAge,q->countryCode,q->passport,q->numberOfFlights,q->totalRes,q->totalPrice) + 1;
    char str[len];
    snprintf(str,len,"--- 1 ---\nname: %s\nsex: %c\nage: %d\ncountry_code: %s\npassport: %s\nnumber_of_flights: %d\nnumber_of_reservations: %d\ntotal_spent: %.3f",q->name,q->sex,q->userAge,q->countryCode,q->passport,q->numberOfFlights,q->totalRes,q->totalPrice);
    return strdup(str);
}

char* toStringQ1User(gpointer data){
    Q1AUXU* q = (Q1AUXU*) data;
    size_t len = snprintf(NULL,0,"%s;%c;%d;%s;%s;%d;%d;%.3f",q->name,q->sex,q->userAge,q->countryCode,q->passport,q->numberOfFlights,q->totalRes,q->totalPrice) + 1;
    char str[len];
    snprintf(str,len,"%s;%c;%d;%s;%s;%d;%d;%.3f",q->name,q->sex,q->userAge,q->countryCode,q->passport,q->numberOfFlights,q->totalRes,q->totalPrice);
    return strdup(str);
}

void destroyQ1U(Q1AUXU* q){
    if(q){
        if(q->name) free(q->name);
        if(q->countryCode) free(q->countryCode);
        if(q->passport) free(q->passport);
        free(q);
    }
}

Q1AUXU* newQ1U(char* name,char sex,int userAge,char* counrtyCode,char* passport,int numberOfFlights,int numberOfReservations, double totalPrice){
    Q1AUXU* q = (Q1AUXU*) malloc(sizeof(struct query1AuxUser));
    q->name = name;
    q->sex = sex;
    q->userAge = userAge;
    q->countryCode = counrtyCode;
    q->passport = passport;
    q->numberOfFlights = numberOfFlights;
    q->totalRes = numberOfReservations;
    q->totalPrice = totalPrice;
    return q;
}

struct query1AuxFlight{
    char* airline;
    char* planeModel;
    char* origin;
    char* destination;
    char* sDDate;
    char* sADate;
    int flightPassengers;
    size_t flightDelay;
};

char* toStringQ1FlightF(gpointer data){
    Q1AUXF* q = (Q1AUXF*) data;
    size_t len = snprintf(NULL,0,"--- 1 ---\nairline: %s\nplane_model: %s\norigin: %s\ndestination: %s\nschedule_departure_date: %s\nschedule_arrival_date: %s\npassengers: %d\ndelay: %lu",q->airline,q->planeModel,q->origin,q->destination,q->sDDate,q->sADate,q->flightPassengers,q->flightDelay) +1;
    char str[len];
    snprintf(str,len,"--- 1 ---\nairline: %s\nplane_model: %s\norigin: %s\ndestination: %s\nschedule_departure_date: %s\nschedule_arrival_date: %s\npassengers: %d\ndelay: %lu",q->airline,q->planeModel,q->origin,q->destination,q->sDDate,q->sADate,q->flightPassengers,q->flightDelay);
    return strdup(str);
}

char* toStringQ1Flight(gpointer data){
    Q1AUXF* q = (Q1AUXF*) data;
    size_t len = snprintf(NULL,0,"%s;%s;%s;%s;%s;%s;%d;%lu",q->airline,q->planeModel,q->origin,q->destination,q->sDDate,q->sADate,q->flightPassengers,q->flightDelay) +1;
    char str[len];
    snprintf(str,len,"%s;%s;%s;%s;%s;%s;%d;%lu",q->airline,q->planeModel,q->origin,q->destination,q->sDDate,q->sADate,q->flightPassengers,q->flightDelay);
    return strdup(str);
}

Q1AUXF* newQ1F(char* airline,char* planeModel,char* origin,char* destination,char* sDDate,char* sADate,int totalPassengers,size_t delay){ 
    Q1AUXF* q = (Q1AUXF*) malloc(sizeof(struct query1AuxFlight));
    q->airline = airline;
    q->planeModel = planeModel;
    q->origin = origin;
    q->destination = destination;
    q->sDDate = sDDate;
    q->sADate = sADate;
    q->flightPassengers = totalPassengers;
    q->flightDelay = delay;
    return q;
}

void destroyQ1F(Q1AUXF* q){
    if(q){
        if(q->airline) free(q->airline);
        if(q->planeModel) free(q->planeModel);
        if(q->origin) free(q->origin);
        if(q->destination) free(q->destination);
        if(q->sDDate) free(q->sDDate);
        if(q->sADate) free(q->sADate);
        free(q);
    }
}

struct query1AuxReservation{
    char* hotelId;
    char* hotelName;
    int hotelStars;
    char* beginDate;
    char* endDate;
    char* includesBreakfast;
    int totalNights;
    double totalPrice;
};

char* toStringQ1ReservationF(gpointer value){
    Q1AUXR* q = (Q1AUXR*) value;
    size_t len = snprintf(NULL,0,"--- 1 ---\nhotel_id: %s\nhotel_name: %s\nhotel_stars: %d\nbegin_date: %s\nend_date: %s\nincludes_breakfast: %s\nnights: %d\ntotal_price: %.3f",q->hotelId,q->hotelName,q->hotelStars,q->beginDate,q->endDate,q->includesBreakfast,q->totalNights,q->totalPrice) +1;
    char str[len];
    snprintf(str,len,"--- 1 ---\nhotel_id: %s\nhotel_name: %s\nhotel_stars: %d\nbegin_date: %s\nend_date: %s\nincludes_breakfast: %s\nnights: %d\ntotal_price: %.3f",q->hotelId,q->hotelName,q->hotelStars,q->beginDate,q->endDate,q->includesBreakfast,q->totalNights,q->totalPrice);
    return strdup(str);
}

char* toStringQ1Reservation(gpointer value){
    Q1AUXR* q = (Q1AUXR*) value;
    size_t len = snprintf(NULL,0,"%s;%s;%d;%s;%s;%s;%d;%.3f",q->hotelId,q->hotelName,q->hotelStars,q->beginDate,q->endDate,q->includesBreakfast,q->totalNights,q->totalPrice) +1;
    char str[len];
    snprintf(str,len,"%s;%s;%d;%s;%s;%s;%d;%.3f",q->hotelId,q->hotelName,q->hotelStars,q->beginDate,q->endDate,q->includesBreakfast,q->totalNights,q->totalPrice);
    return strdup(str);
}

Q1AUXR* newQ1R(char* hotelId,char* hotelName,int hotelStars,char* beginDate,char* endDate,char* includesBreakfast,int totalNights,double totalPrice){
    Q1AUXR* q = (Q1AUXR*) malloc(sizeof(struct query1AuxReservation));
    q->hotelId = hotelId;
    q->hotelName = hotelName;
    q->hotelStars = hotelStars;
    q->beginDate = beginDate;
    q->endDate = endDate;
    q->includesBreakfast = includesBreakfast;
    q->totalNights = totalNights;
    q->totalPrice = totalPrice;
    return q;
}

void destroyQ1R(Q1AUXR* q){
    if(q){
        if(q->hotelId) free(q->hotelId);
        if(q->hotelName) free(q->hotelName);
        if(q->beginDate) free(q->beginDate);
        if(q->endDate) free(q->endDate);
        if(q->includesBreakfast) free(q->includesBreakfast);
        free(q);
    }
}

void runQuery1(Tables* t,char* id,int format,FILE* output){
    // flag que indica se já executamos a query 
    int flag = 1;
    FAC* fac = newFAC(output,1,1,format);

    if (isActiveUserCTG(getUserCTG(t),id)) {
        Q1AUXU* q = userInfoQ1CTG(t,id);
        
        if (format) printResult(fac,stringToQueue(q,toStringQ1UserF));
        else printResult(fac,stringToQueue(q,toStringQ1User));
        
        destroyQ1U(q);
        flag = 0;
    }
    if (flag){
        if (existingFlight(getFlightCTG(t),id)){

            Q1AUXF* q = flightInfoQ1CTG(t,id);

            if (format) printResult(fac,stringToQueue(q,toStringQ1FlightF));
            else printResult(fac,stringToQueue(q,toStringQ1Flight));

            destroyQ1F(q);
            flag = 0;
        }
    }
    if (flag){
        if (existsReservation(getReservationCTG(t),id)){
            
            Q1AUXR* q = reservationInfoQ1CTG(t,id);

            if (format) printResult(fac,stringToQueue(q,toStringQ1ReservationF));
            else printResult(fac,stringToQueue(q,toStringQ1Reservation));

            destroyQ1R(q);
        }
    }
    if(id) free(id);
    if(fac) destroyFAC(fac);
}

void formatQuery1(Tables* t,char* line,int format,FILE* output){
    char* id;
    char* token;
    int field = 0;
    while((token = strsep(&line," "))){
        switch (field){
        case 1:
            id = strdup(removeEnter(token));
            break;
        }
        field++;
    }

    runQuery1(t,id,format,output);
}