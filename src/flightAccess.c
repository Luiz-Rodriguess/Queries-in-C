#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <glib.h>
#include "../include/flightAccess.h"
#include "../include/userAccess.h"
#include "../include/flights.h"
#include "../include/statistics.h"
#include "../include/util.h"
#include "../include/table.h"
#include "../include/query1.h"
#include "../include/query5.h"
#include "../include/query6.h"
#include "../include/query7.h"
#include "../include/query10.h"

struct flightCTG {
    GHashTable* flightTable;
    GHashTable* flightIds;
    GHashTable* airportFlights;
};

FlightCTG* newFlightCTG(char* flightCSV){
    FlightCTG* ctg = (FlightCTG*) malloc(sizeof(struct flightCTG));
    FlightAux* aux = hashAtributeFlight(flightCSV);
    ctg->flightTable = getFlightTableAux(aux);
    ctg->flightIds = getFlightIdsAux(aux);
    ctg->airportFlights = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,destroyAux);
    if (aux) free(aux);
    return ctg;
}

void destroyFlightCTG(FlightCTG* ctg){
    if(ctg){
        if (ctg->flightTable) destroyFlightTable(ctg->flightTable);
        if (ctg->flightIds) destroyFlightIds(ctg->flightIds);
        if (ctg->airportFlights) g_hash_table_destroy(ctg->airportFlights);
        free(ctg);
    }
}

void removeInvalidEntries(FlightCTG* flightCTG, UserCTG* userCTG){
    removeExceededTotalSeats(flightCTG->flightTable,flightCTG->flightIds,flightCTG->airportFlights,userCTG);
}

void addUserToFlightCTG(FlightCTG* ctg, char* userId, char* flightId){
    addUserToUserSet(ctg->flightTable,userId, flightId);
}

gboolean existingFlight(FlightCTG* ctg,char* flightId){
    return g_hash_table_contains(ctg->flightIds,flightId);
}

char* getFlightIdCTG(FlightCTG* ctg, char* flightId){
    return (char*) g_hash_table_lookup(ctg->flightIds,flightId);
}

Q1AUXF* flightInfoQ1CTG(gpointer tables, char* flightId){
    Tables* t = (Tables*) tables;
    FlightCTG* ctg = (FlightCTG*) getFlightCTG(t);
    Flight* f = g_hash_table_lookup(ctg->flightTable,flightId);
    char* airline = getFlightAirline(f);
    char* planeModel = getFlightPlaneModel(f);
    char* origin = getFlightOrigin(f);
    char* destination = getFlightDestination(f);
    char* sDDAte = getScheduleDepartureDate(f);
    char* sADate = getFlightScheduleArrivalDate(f);
    int totalPassengers = getFlightPassengers(f);
    size_t delay = getFlightDelay(f);
    return newQ1F(airline,planeModel,origin,destination,sDDAte,sADate,totalPassengers,delay);
}

char* getSDDateCTG(FlightCTG* ctg, char* flightId){
    Flight* f = g_hash_table_lookup(ctg->flightTable,flightId);
    return getScheduleDepartureDate(f);
}

/* função que retorna a lista ordenada dos valores que deverão ser impressos na query 5 */
int listFLightsOrigin(FlightCTG* ctg,char* airportName,char* beginDate,char* endDate,GTree* tree){
    GHashTable* flightsFromAirport = g_hash_table_lookup(ctg->airportFlights,airportName);
    
    GHashTableIter iter;
    gpointer key, value;

    if (!flightsFromAirport) return 0;
    
    g_hash_table_iter_init(&iter, flightsFromAirport);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        char* flightId = (char*) value;
        Flight* f = (Flight*) g_hash_table_lookup(ctg->flightTable,flightId);
            
        char* sDDate = getScheduleDepartureDate(f);
            
        if(endAfterStartDateTime(sDDate,beginDate) && endAfterStartDateTime(endDate,sDDate)){        

            char* airline = getFlightAirline(f);
            char* destination = getFlightDestination(f);
            char* id = getFlightId(f);
            char* planeModel = getFlightPlaneModel(f);
            char* sDDate = getScheduleDepartureDate(f);

            Q5AUX* q = newQ5(airline,destination,id,planeModel,sDDate);
            g_tree_insert(tree,q,NULL);
        }
        if(sDDate) free(sDDate);
    }
    return 1;
}

void addPassengersToAirports(GHashTable* tabelaQ6, Flight* f){
    char* flightOrigin = getFlightOrigin(f);
    char* flightDestination = getFlightDestination(f);
    
    // procuramos se o aeroporto de origem já está na tabela auxiliar
    Q6AUX* q = g_hash_table_lookup(tabelaQ6,flightOrigin);
    if(q == NULL){

        // se a origem não estiver na tabela criamos uma nova entrada com a chave sendo f->origin
        q = newQ6(flightOrigin);
        g_hash_table_insert(tabelaQ6,getAirportQ6(q),q);
    }

    // adicionamos o número de passageiros ao aeroporto origem

    int numberOfPassengersToAdd = getFlightPassengers(f);

    incPassengers(q,numberOfPassengersToAdd);

    // procuramos se o aeroporto de destino já está na tabela auxiliar
    q = g_hash_table_lookup(tabelaQ6,flightDestination);
    if(q == NULL){

        // se o destino não estiver na tabela criamos uma nova entrada com a chave sendo f->origin
        q = newQ6(flightDestination);
        g_hash_table_insert(tabelaQ6,getAirportQ6(q),q);
    }

    // adicionamos o número de passageiros ao aeroporto destino
    incPassengers(q,numberOfPassengersToAdd);

    if (flightOrigin) free(flightOrigin);
    if (flightDestination) free(flightDestination);
}

/* Função que cria uma lista com todos os aeroportos daquele ano */
void passengersOnYear(FlightCTG* ctg, char* year,GTree* tree){

    // tabela auxiliar para a query 6
    GHashTable* tabelaQ6 = g_hash_table_new(g_str_hash,g_str_equal);

    GHashTableIter iter;
    gpointer key,value;

    g_hash_table_iter_init(&iter,ctg->flightTable);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        Flight* f = (Flight*) value;
        char* sDDate = getScheduleDepartureDate(f);

        // verificamos se a data do flight está no ano especificado no input da query
        if(isInOpt(sDDate,year)){
            
            // se o flight estiver dentro do ano adicionamos os passageiros a tabela
            addPassengersToAirports(tabelaQ6,f);
        }
        if(sDDate) free(sDDate);
    }

    g_hash_table_iter_init(&iter,tabelaQ6);

    // após termos obtido todos os passageiros daquele ano inserimos suas cópias na lista ordenadas
    while(g_hash_table_iter_next(&iter,&key,&value)){
        Q6AUX* q = (Q6AUX*) value;
        g_tree_insert(tree,q,NULL);
    }

    // liberamos a memória atribuída à tabela auxiliar
    g_hash_table_destroy(tabelaQ6);

}

typedef struct airportDelays{
    GList* delays;
    char* airport;
}AIRD;

AIRD* newAIRD(char* airport){
    AIRD* a = (AIRD*) malloc(sizeof(struct airportDelays));
    a->delays = NULL;
    a->airport = strdup(airport);
    return a;
}

void destroyAIRD(gpointer value){
    AIRD* a = (AIRD*) value;
    if(a){
        if(a->airport) free(a->airport);
        g_list_free(a->delays);
        free(a);
    }
}

gint compareDelay(gconstpointer a, gconstpointer b){
    size_t iA = (size_t) (uintptr_t) a;
    size_t iB = (size_t) (uintptr_t) b;

    if(iA < iB) return -1;
    return 1;
}

void getMedianDelays(FlightCTG* ctg,GTree* tree){
    
    // tabela auxiliar para a query 7
    GHashTable* tabelaQ7 = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,destroyAIRD);
    
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter,ctg->flightTable);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        Flight* f = (Flight*) value;

        char* flightOrigin = getFlightOrigin(f);

        AIRD* aird = g_hash_table_lookup(tabelaQ7,flightOrigin);
        if(aird == NULL){

            aird = newAIRD(flightOrigin);
            g_hash_table_insert(tabelaQ7,aird->airport,aird);
        }

        aird->delays = g_list_insert_sorted(aird->delays,(gpointer)(uintptr_t) getFlightDelay(f),compareDelay);
        if (flightOrigin) free(flightOrigin);
    }

    g_hash_table_iter_init(&iter,tabelaQ7);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        AIRD* aird = (AIRD*) value;
        int n = g_list_length(aird->delays);
        Q7AUX* q = newQ7(aird->airport);
        size_t delay = 0;

        if (n > 0){
            int index = n / 2;
            delay = (size_t) (uintptr_t) g_list_nth_data(aird->delays,index);
            if(n % 2 == 0){
                index--;
                delay += (size_t) (uintptr_t) g_list_nth_data(aird->delays,index);
                delay /= 2;
            }
        }

        setDelay(q,delay);
        g_tree_insert(tree,q,NULL);
    }
    
    // liberamos a memória atribuída à tabela auxiliar
    g_hash_table_destroy(tabelaQ7);
}

// query10

typedef struct uniquePassengers{
    char* ymd;
    GHashTable* uniqueTable;
}UPass;

UPass* newUP(char* ymd){
    UPass* new = malloc(sizeof(struct uniquePassengers));
    new->ymd = strdup(ymd);
    new->uniqueTable = g_hash_table_new_full(g_str_hash,g_str_equal,free,NULL);
    return new;
} 

void destroyUP(gpointer value){
    UPass* up = (UPass*) value;
    if(up){
        if(up->ymd) free(up->ymd);
        if(up->uniqueTable) g_hash_table_destroy(up->uniqueTable);
        free(up);
    }
}

void flightStatsQ10(FlightCTG* ctg,GHashTable* tabelaQ10,char* yearOpt,char* monthOpt){
    int flag = 1;
    if(monthOpt) flag = 3; // Ano e mês como opção na query
    else if (yearOpt) flag = 2; // Ano como opção na query

    GHashTableIter iter;
    gpointer key, value;

    GHashTable* upTable = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,destroyUP);

    g_hash_table_iter_init(&iter,ctg->flightTable);
    while(g_hash_table_iter_next(&iter,&key,&value)){
        Flight* f = (Flight*) value;
        char* sDDate = getScheduleDepartureDate(f);
        switch(flag){
            case 1:{
            // caso em que não foi especificado nem ano nem mês
                char* year = getYearFromDate(sDDate);
                Q10AUX* q = g_hash_table_lookup(tabelaQ10,year);        
                if(q == NULL){
                    q = newQ10(year);
                    g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                }
                incQ10Flights(q);
                incQ10TotalPassengers(q,getFlightPassengers(f));
                    
                UPass* up = g_hash_table_lookup(upTable,year);

                if(up  == NULL){
                    up = newUP(year);
                    g_hash_table_insert(upTable,up->ymd,up);
                }

                addUniquePassengers(up->uniqueTable,f);
                setQ10UniquePassengers(q,g_hash_table_size(up->uniqueTable));

                if(year) free(year);
                break;
            }
            case 2:{
                if(isInOpt(sDDate,yearOpt)){
                    char* month = getMonthFromDate(sDDate);
                    Q10AUX* q = g_hash_table_lookup(tabelaQ10,month);
                    if(q == NULL){
                        q = newQ10(month);
                        g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                    }
                    incQ10Flights(q);
                    incQ10TotalPassengers(q,getFlightPassengers(f));

                    UPass* up = g_hash_table_lookup(upTable,month);

                    if(up  == NULL){
                        up = newUP(month);
                        g_hash_table_insert(upTable,up->ymd,up);
                    }
                    addUniquePassengers(up->uniqueTable,f);
                    setQ10UniquePassengers(q,g_hash_table_size(up->uniqueTable));
                    
                    if(month) free(month);
                }
                break;
            }
            case 3:{
                char opt [strlen(yearOpt) + strlen(monthOpt) + 2];
                sprintf(opt,"%s/%s",yearOpt,monthOpt);
                // verificamos se o flight está no mesmo mês como input na query 10
                if(isInOpt(sDDate,opt)){
                    // as operações são análogas as descritas no primeiro caso
                    char* day = getDayFromDate(sDDate);
                    Q10AUX* q = g_hash_table_lookup(tabelaQ10,day);
                    if(q == NULL){
                        q = newQ10(day);
                        g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                    }
                    incQ10Flights(q);
                    incQ10TotalPassengers(q,getFlightPassengers(f));

                    UPass* up = g_hash_table_lookup(upTable,day);

                    if(up  == NULL){
                        up = newUP(day);
                        g_hash_table_insert(upTable,up->ymd,up);
                    }
                    addUniquePassengers(up->uniqueTable,f);
                    setQ10UniquePassengers(q,g_hash_table_size(up->uniqueTable));

                    if(day) free(day);
                }
            }
        }
        if (sDDate) free(sDDate);
    }
    g_hash_table_destroy(upTable);
}
