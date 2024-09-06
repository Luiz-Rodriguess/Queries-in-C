#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>
#include "../include/reservations.h"
#include "../include/userAccess.h"
#include "../include/statistics.h"
#include "../include/util.h"
#include "../include/parser.h"
#include "../include/table.h"
#include "../include/query2.h"
#include "../include/query1.h"
#include "../include/query10.h"

struct reservation{
    char* id;
    char* userId;
    char* hotelId;
    char* hotelName;
    int hotelStars;
    int cityTax;
    char* address;
    char* beginDate;
    char* endDate;
    int pricePerNight;
    char* includesBreakfast;
    char* roomDetails;
    char* rating;
    char* comment;
};

Reservation* createReservation(){
    Reservation* r = (Reservation*) malloc(sizeof (struct reservation));
    r->id = NULL;
    r->userId = NULL;
    r->hotelId = NULL;
    r->hotelName = NULL;
    r->hotelStars = 0;
    r->cityTax = 0;
    r->address = NULL;
    r->beginDate = NULL;
    r->endDate = NULL;
    r->pricePerNight = 0;
    r->includesBreakfast = NULL;
    r->roomDetails = NULL;
    r->rating = NULL;
    r->comment = NULL;
    return r;
}

void destroyReservation(Reservation* r){
    if (r){
        if(r->id) {free(r->id);r->id = NULL;}
        if(r->hotelId) {free(r->hotelId);r->hotelId = NULL;}
        if(r->hotelName) {free(r->hotelName);r->hotelName = NULL;}
        if(r->address) {free(r->address);r->address = NULL;}
        if(r->beginDate) {free(r->beginDate);r->beginDate = NULL;}
        if(r->endDate) {free(r->endDate);r->endDate = NULL;}
        if(r->includesBreakfast) {free(r->includesBreakfast);r->includesBreakfast = NULL;}
        if(r->roomDetails) {free(r->roomDetails);r->roomDetails = NULL;}
        if(r->rating) {free(r->rating);r->rating = NULL;}
        if(r->comment) {free(r->comment);r->comment = NULL;}
        free(r);
        r = NULL;
    }
}

void addHotelReservation(GHashTable* hotelReservations,char* hotelId,char* reservationId){
    if (!g_hash_table_contains(hotelReservations,hotelId)){
        GHashTable* newLog = g_hash_table_new(g_str_hash,g_str_equal);
        g_hash_table_insert(hotelReservations,hotelId,newLog);
    }
    GHashTable* log = g_hash_table_lookup(hotelReservations,hotelId);
    g_hash_table_add(log,reservationId);
}

typedef struct atributeAux{
    UserCTG* ctg;
    GHashTable* hotelReservations;
}Atribute;

struct reservationAux{
    GHashTable* reservationTable;
    GHashTable* hotelReservations;
};

GHashTable* getReservationTable(ReservationAux* aux){ return aux->reservationTable;}

GHashTable* getHotelReservations(ReservationAux* aux) { return aux->hotelReservations;}

void hashDestroyReservation(gpointer value){
    destroyReservation((Reservation*) value);
}

Reservation* atributeReservation(char* line,FILE* resError,Atribute* at){
    
    char* token;
    Reservation* r = createReservation();
    int field = 0;
    while ((token = strsep(&line, ";"))){
        switch (field) {
            case 0:
                if(strlen(token) < 1){
                    destroyReservation(r);
                    return NULL;
                }
                r->id = strdup(token);
                break;
            case 1:
                if(strlen(token) < 1){
                    destroyReservation(r);
                    return NULL;
                }
                if(!existingUser(at->ctg,token)){
                    destroyReservation(r);
                    return NULL;
                }
                r->userId = getUserIdCTG(at->ctg,token);
                break;
            case 2:
                if(strlen(token) < 1){
                    destroyReservation(r);
                    return NULL;
                }
                r->hotelId = strdup(token);
                break;
            case 3:
                if(strlen(token) < 1){
                    destroyReservation(r);
                    return NULL;
                }
                r->hotelName = strdup(token);
                break;
            case 4:
                if (strlen(token)<1 || !checkInt(token) || atoi(token)<1 || atoi(token)>5){
                    destroyReservation(r);
                    return NULL;
                }
                r->hotelStars = atoi(token);
                break;
            case 5:
                if (strlen(token)<1 || !checkInt(token) || atoi(token)<0){
                    destroyReservation(r);
                    return NULL;
                }
                r->cityTax = atoi(token); 
                break;
            case 6:
                if(strlen(token) < 1){
                    destroyReservation(r);
                    return NULL;
                }
                r->address = strdup(token);
                break;
            case 7:
                if(!checkDate(token)){
                    destroyReservation(r);
                    return NULL;
                }
                r->beginDate = strdup(token);
                break;
            case 8:
                if(!checkDate(token)){
                    destroyReservation(r);
                    return NULL;
                }
                r->endDate = strdup(token);
                break;
            case 9:
                if (strlen(token)<1 || !checkInt(token) || atoi(token)<0){
                    destroyReservation(r);
                    return NULL;
                }
                r->pricePerNight = atoi(token);
                break;
            case 10:
                if(strcasecmp(token,"t") == 0 || strcasecmp(token,"true") == 0 || strcasecmp(token,"1") == 0){
                    r->includesBreakfast = strdup("True");
                } else if(strcasecmp(token,"f") == 0 || strcasecmp(token,"false") == 0 || strcasecmp(token,"0") == 0 || strlen(token) == 0){
                    r->includesBreakfast = strdup("False");
                } else{
                    destroyReservation(r);
                    return NULL;   
                }
                break;
            case 11:
                r->roomDetails = strdup(token);
                break;
            case 12:
                if ((strcmp(token,"") != 0)){
                    if (!checkInt(token) || atoi(token)<1 || atoi(token)>5){
                        destroyReservation(r);
                        return NULL;
                    }
                    r->rating = strdup(token);
                }
                else{
                    destroyReservation(r);
                    return NULL;
                }
                break;
            case 13:
                r->comment = strdup(token);
                break;
        }
        field++;
    }
    if(endAfterStartDate(r->beginDate,r->endDate) || strcmp(r->beginDate,r->endDate) == 0){
        destroyReservation(r);
        return NULL;
    }
    addReservationToUser(at->ctg,r->userId,r->id);
    addHotelReservation(at->hotelReservations,r->hotelId,r->id);
    return r;
}

/* Função de população da tabela de reservations */
void fillReservationTable(GHashTable* reservationTable, char* line, FILE* resErrors,gpointer atributes){
    Atribute* at = (Atribute*) atributes;
    char* error = strdup(line);
    Reservation* r = atributeReservation(line, resErrors,at);
    if (r) g_hash_table_insert(reservationTable,(r->id),r);
    else fputs(error,resErrors);
    free(error);
}

/*  Função que deve ser chamada para ser possível criar uma tabela nova */
ReservationAux* hashAtributeReservation(Tables* t,char* reservationCSV){

    // criamos a tabela que contém as informações lidas no csv
    GHashTable* reservationTable = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,hashDestroyReservation);
    GHashTable* hotelReservations = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,destroyAux);

    Atribute* at = (Atribute*) malloc(sizeof(struct atributeAux));
    at->ctg = getUserCTG(t);
    at->hotelReservations = hotelReservations;

    // abrimos o ficheiro de erros
    FILE* resErrors = fopen("Resultados/reservations_errors.csv","w");

    // chamamos a função parse para preencher a tabela
    parse(reservationCSV,reservationTable,fillReservationTable,resErrors,at);

    fclose(resErrors);

    if(at) free(at);
    ReservationAux* aux = (ReservationAux*) malloc(sizeof(struct reservationAux));
    aux->reservationTable = reservationTable;
    aux->hotelReservations = hotelReservations;
    return aux;
}

void destroyReservationTable(GHashTable* reservationTable){
    g_hash_table_destroy(reservationTable);
}

char* getBeginDate(Reservation* r){ return strdup(r->beginDate);}
char* getEndDate(Reservation* r){ return strdup(r->endDate);}

// função que calcula o número de dias entre duas datas
int getNights(char* bDate, char* eDate){
    int bDay, eDay;
    sscanf(bDate,"%*d/%*d/%d",&bDay);
    sscanf(eDate,"%*d/%*d/%d",&eDay);

    // garantido no enunciado que as reservas são no mesmo mês
    return eDay - bDay;
}

// função que calcula o número de noites de uma reservation
int getReservationNights(Reservation* r){
    char* bDate = getBeginDate(r);
    char* eDate = getEndDate(r);
    
    int totalNights = getNights(bDate, eDate);

    free(bDate);
    free(eDate);

    return totalNights;
}

int getPricePerNight(Reservation* r){
    return r->pricePerNight;
}

int getCityTax(Reservation* r){
    return r->cityTax;
}

// função que calcula o preço total de uma reserva
double getTotalPriceReservation(Reservation* r,int totalNights){
    double priceWithoutTax = (getPricePerNight(r) * totalNights);
    double totalPrice = priceWithoutTax + (priceWithoutTax/100) * getCityTax(r);
    return totalPrice;
}

/* ------------------------------------- Getters --------------------------------------- */

char* getReservationId(Reservation* r) {return strdup(r->id);}
char* getReservationHotelId(Reservation* r) { return strdup(r->hotelId);}
char* getReservationHotelName(Reservation* r) { return strdup(r->hotelName);}
int getReservationHotelStars(Reservation* r) { return r->hotelStars;}
char* getReservationBeginDate(Reservation* r) { return strdup(r->beginDate);}
char* getReservationEndDate(Reservation* r) { return strdup(r->endDate);}
char* getReservationIncludesBreakfast(Reservation* r) {return strdup(r->includesBreakfast);}
double getReservationRating(Reservation* r) {return atof(r->rating);}
char* getReservationRatingAsString(Reservation* r) {return strdup(r->rating);}
char* getReservationUserId(Reservation* r) {return strdup(r->userId);}

double userResInfoQ1(GHashTable* reservationTable,GHashTable* userReservations){
    double total = 0.0;
    GHashTableIter iter;
    gpointer key,value;

    g_hash_table_iter_init(&iter, userReservations);

    while(g_hash_table_iter_next(&iter,&key, &value)){
        char* reservationId = (char*) value;

        Reservation* aux = (Reservation*) g_hash_table_lookup(reservationTable,reservationId);
        total += getTotalPriceReservation(aux,getReservationNights(aux));
    }
    return total;
}

/* -------------------------------------Query8--------------------------------------- */

int compareDates(char* date1, char* date2){
    // Extract year, month, and day from date1
    int year1, month1, day1;
    sscanf(date1, "%d/%d/%d", &year1, &month1, &day1);

    // Extract year, month, and day from date2
    int year2, month2, day2;
    sscanf(date2, "%d/%d/%d", &year2, &month2, &day2);

    // Compare years
    if (year1 < year2) {
        return -1;
    } else if (year1 > year2) {
        return 1;
    }

    // If years are equal, compare months
    if (month1 < month2) {
        return -1;
    } else if (month1 > month2) {
        return 1;
    }

    // If months are equal, compare days
    if (day1 < day2) {
        return -1;
    } else if (day1 > day2) {
        return 1;
    }

    // Dates are equal
    return 0;
}

char* returnMostRecentDate(char* date1, char* date2){
    // Convert date strings to integers

    return compareDates(date1, date2) < 1 ? date1 : date2; // garantido no enunciado que as reservas são no mesmo mês
}

char* returnLatterDate(char* date1, char* date2){
    // Convert date strings to integers

    return compareDates(date1, date2) < 1 ? date2 : date1; // garantido no enunciado que as reservas são no mesmo mês
}

int getReservationNightsWithBeginAndEndDate(Reservation* r, char* eBeginInput, char* eDateInput){
    char* bDate = getBeginDate(r);
    char* eDate = getEndDate(r);


    char* actualBeginDate = returnLatterDate(bDate, eBeginInput);
    char* actualEndDate = returnMostRecentDate(eDate, eDateInput);
    
    int totalNights = getNights(actualBeginDate, actualEndDate);

    if(actualEndDate == eDateInput){
        totalNights += 1;
    }

    free(bDate);
    free(eDate);

    return totalNights;
}

// Function to remove a character from a string
void removeChar(char *str, char c) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != c) dst++;
    }
    *dst = '\0';
}

// Function to check if two date ranges intersect
int doDatesIntersect(const char *start1, const char *end1, const char *start2, const char *end2) {
    // Remove '/' characters
    char cleanStart1[11], cleanEnd1[11], cleanStart2[11], cleanEnd2[11];
    strcpy(cleanStart1, start1);
    strcpy(cleanEnd1, end1);
    strcpy(cleanStart2, start2);
    strcpy(cleanEnd2, end2);
    removeChar(cleanStart1, '/');
    removeChar(cleanEnd1, '/');
    removeChar(cleanStart2, '/');
    removeChar(cleanEnd2, '/');

    // Convert date strings to integers
    int intStart1 = atoi(cleanStart1);
    int intEnd1 = atoi(cleanEnd1);
    int intStart2 = atoi(cleanStart2);
    int intEnd2 = atoi(cleanEnd2);

    // Check for intersection
    if (intEnd1 < intStart2 || intStart1 > intEnd2) {
        // No intersection
        return 0;
    } else {
        // Intersection
        return 1;
    }
}

int doesReservationIntersect(Reservation* r,char* beginDate, char* endDate) {
    return doDatesIntersect(r->beginDate,r->endDate,beginDate,endDate);
}

int getTotalPriceQ8(Reservation* r,char* beginDate, char* endDate){
    return r->pricePerNight * getReservationNightsWithBeginAndEndDate(r, beginDate, endDate);
}
