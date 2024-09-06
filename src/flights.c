#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/flightAccess.h"
#include "../include/statistics.h"
#include "../include/flights.h"
#include "../include/util.h"
#include "../include/parser.h"
#include "../include/passengers.h"
#include "../include/table.h"
#include "../include/query10.h"

struct flight{
    char* id;
    char* airline;
    char* planeModel;
    unsigned int totalSeats;
    char* origin;
    char* destination;
    char* scheduleDepartureDate;
    char* scheduleArrivalDate;
    char* realDepartureDate;
    char* realArrivalDate;
    char* pilot;
    char* copilot;
    char* notes;
    GHashTable* usersInFlight;
};

Flight* createFlight(){
    Flight* f = (Flight*) malloc(sizeof (struct flight));
    f->id = NULL;
    f->airline = NULL;
    f->planeModel = NULL;
    f->totalSeats = 0;
    f->origin = NULL;
    f->destination = NULL;
    f->scheduleDepartureDate = NULL;
    f->scheduleArrivalDate = NULL;
    f->realDepartureDate = NULL;
    f->realArrivalDate = NULL;
    f->pilot = NULL;
    f->copilot = NULL;
    f->notes = NULL;
    f->usersInFlight = g_hash_table_new(g_str_hash,g_str_equal);
    return f;
}

void destroyFlight(Flight* f){
    if (f){
        if (f->airline) {free(f->airline);f->airline = NULL;}
        if (f->planeModel) {free(f->planeModel);f->planeModel = NULL;}
        if (f->origin) {free(f->origin);f->origin = NULL;}
        if (f->destination) {free(f->destination);f->destination = NULL;}
        if (f->scheduleDepartureDate) {free(f->scheduleDepartureDate);f->scheduleDepartureDate = NULL;}
        if (f->scheduleArrivalDate) {free(f->scheduleArrivalDate);f->scheduleArrivalDate = NULL;}
        if (f->realDepartureDate) {free(f->realDepartureDate);f->realDepartureDate = NULL;}
        if (f->realArrivalDate) {free(f->realArrivalDate);f->realArrivalDate = NULL;}
        if (f->pilot) {free(f->pilot);f->pilot = NULL;}
        if (f->copilot) {free(f->copilot);f->copilot = NULL;}
        if (f->notes) {free(f->notes);f->notes = NULL;}
        if (f->usersInFlight) {g_hash_table_destroy(f->usersInFlight);f->usersInFlight = NULL;}
        free(f);
        f = NULL;
    }
}

void hashDestroyFlight(gpointer value){
    destroyFlight((Flight*) value);
}

Flight* atributeFlight(char* line,GHashTable* flightIds){
    char* flightId;
    char* token;
    Flight* f = createFlight();
    int field = 0;
    while ((token = strsep(&line, ";"))){
        switch (field) {
            case 0:
                if(strlen(token) < 1){
                    destroyFlight(f);
                    return NULL;
                }
                flightId = strdup(token);
                f->id = flightId;
                break;
            case 1:
                if(strlen(token) < 1){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->airline = strdup(token); 
                break;
            case 2:
                if(strlen(token) < 1){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->planeModel = strdup(token); 
                break;
            case 3:{
                if (strlen(token)<1 || !checkInt(token)){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                unsigned int aux = atoi(token);
                f->totalSeats = aux;
                break;
            }
            case 4:
                if ((strlen(token) != 3) || !isalpha(token[0]) || !isalpha(token[1]) || !isalpha(token[2])){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                for(unsigned long i=0;i<strlen(token);i++){
                    token[i] = toupper(token[i]);
                }
                f->origin = strdup(token);
                break;
            case 5:
                if ((strlen(token) != 3) || !isalpha(token[0]) || !isalpha(token[1]) || !isalpha(token[2])){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                for(unsigned long i=0;i<strlen(token);i++){
                    token[i] = toupper(token[i]);
                }
                f->destination = strdup(token); 
                break;
            case 6:
                if (!checkDateTime(token)){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->scheduleDepartureDate = strdup(token);
                break;
            case 7:
                if (!checkDateTime(token)){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->scheduleArrivalDate = strdup(token);
                break;
            case 8:
                if (!checkDateTime(token)){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->realDepartureDate = strdup(token);
                break;
            case 9:
                if (!checkDateTime(token)){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->realArrivalDate = strdup(token);
                break;
            case 10:
                if(strlen(token) < 1){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->pilot = strdup(token);
                break;
            case 11:
                if(strlen(token) < 1){
                    destroyFlight(f);
                    free(flightId);
                    return NULL;
                }
                f->copilot = strdup(token);
                break;
            case 12:
                f->notes = removeEnter(strdup(token));
                break;
        }
        field++;
    }
    if(!endAfterStartDateTime(f->scheduleArrivalDate, f->scheduleDepartureDate)){
        destroyFlight(f);
        free(flightId);
        return NULL;
    }
    if(!endAfterStartDateTime(f->realArrivalDate,f->realDepartureDate)){
        destroyFlight(f);
        free(flightId);
        return NULL;
    }
    g_hash_table_add(flightIds,flightId);
    return f;
}

void fillFlightTable(GHashTable* flightTable, char* line, FILE* flightErrors,gpointer userData){
    char* error = strdup(line);
    GHashTable* flightIds = (GHashTable*) userData;
    Flight* f = atributeFlight(line,flightIds);
    if (f) g_hash_table_insert(flightTable,(f->id),f);
    else fputs(error,flightErrors);
    free(error);
}

struct flightAux{
    GHashTable* flightTable;
    GHashTable* flightIds;
};

GHashTable* getFlightTableAux(FlightAux* aux){
    return aux->flightTable;
}

GHashTable* getFlightIdsAux(FlightAux* aux){
    return aux->flightIds;
}

FlightAux* hashAtributeFlight(char* flightCSV){

    GHashTable* flightTable = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,hashDestroyFlight);
    GHashTable* flightIds = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,free);

    FILE* flightErrors = fopen("Resultados/flights_errors.csv", "w");

    parse(flightCSV,flightTable,fillFlightTable,flightErrors,flightIds);

    fclose(flightErrors);

    FlightAux* aux = (FlightAux*) malloc(sizeof(struct flightAux));
    aux->flightTable = flightTable;
    aux->flightIds = flightIds;

    return aux;
}

void addUserToUserSet(GHashTable* flightTable,char* userId, char* flightId){
    Flight* f = g_hash_table_lookup(flightTable,flightId);
    if (f) g_hash_table_add(f->usersInFlight,userId);
}

/* Função para escrever os erros referentes ao excesso de passengers */
void writeErrorFlight(Flight* f,FILE* flightErrors){
    fprintf(flightErrors,"%s;%s;%s;%d;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",f->id,f->airline,f->planeModel,f->totalSeats,f->origin,f->destination,f->scheduleDepartureDate,f->scheduleArrivalDate,f->realDepartureDate,f->realArrivalDate,f->pilot,f->copilot,f->notes);
}

/* Função para escrever os erros referentes ao excesso de passengers */
void writeErrorPassenger(char* flightId,GHashTable* usersInFlight,FILE* passengerErrors){
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter,usersInFlight);
    while(g_hash_table_iter_next(&iter,&key,&value)){
        char* userId = (char*) value;
        fprintf(passengerErrors,"%s;%s\n",flightId,userId);
    }
}

void destroyFlightTable(GHashTable* flightTable){
    g_hash_table_destroy(flightTable);
}

void destroyFlightIds(GHashTable* flightIds){
    g_hash_table_destroy(flightIds);
}

void addFlightsToUserAux(UserCTG* userCTG, GHashTable* usersInFLight, char* flightId){
    GHashTableIter iter;
    gpointer key,value;

    g_hash_table_iter_init(&iter,usersInFLight);

    while (g_hash_table_iter_next(&iter,&key,&value)){
        char* userId = (char*) value;
        addFlightToUser(userCTG,userId,flightId);
    }
}

void addAirportFlights(GHashTable* airportFlights, char* airportName, char* flightId){
    if (!g_hash_table_contains(airportFlights,airportName)){
        GHashTable* newLog = g_hash_table_new(g_str_hash,g_str_equal);
        g_hash_table_insert(airportFlights,airportName,newLog);
    }
    GHashTable* log = g_hash_table_lookup(airportFlights,airportName);
    g_hash_table_add(log,flightId);
}

void removeExceededTotalSeats(GHashTable* flightTable, GHashTable* flightIds,GHashTable* airportFlights,UserCTG* userCTG){
    GHashTableIter iter;
    gpointer key,value;

    FILE* flightErrors = fopen("Resultados/flights_errors.csv","a");
    FILE* passengerErrors = fopen("Resultados/passengers_errors.csv","a");

    g_hash_table_iter_init(&iter,flightTable);

    while (g_hash_table_iter_next(&iter,&key,&value)){
        Flight* aux = (Flight*) value;
        if (g_hash_table_size(aux->usersInFlight)>aux->totalSeats){
            writeErrorFlight(aux,flightErrors);

            writeErrorPassenger(aux->id,aux->usersInFlight,passengerErrors);

            g_hash_table_remove(flightIds,aux->id);
            g_hash_table_iter_remove(&iter);

        }else{
            addFlightsToUserAux(userCTG,aux->usersInFlight,aux->id);
            addAirportFlights(airportFlights,aux->origin,aux->id);
        }
    }

    fclose(flightErrors);
    fclose(passengerErrors);
}

/* Função que calcula o intervalo de tempo entre duas datetime em segundos */
size_t getDelay(char* sDepDate,char* rDepDate){
    size_t sDay, sHour, sMinute, sSecond;
    size_t rDay, rHour, rMinute, rSecond;
    
    sscanf(sDepDate,"%*d/%*d/%lu %lu:%lu:%lu",&sDay,&sHour,&sMinute,&sSecond);
    sscanf(rDepDate,"%*d/%*d/%lu %lu:%lu:%lu",&rDay,&rHour,&rMinute,&rSecond);
    
    // Passamos os valores lidos para um tempo total em segundos
    
    size_t timeInSecondsSDDate = 86400* sDay + 3600 * sHour + 60 * sMinute + sSecond;
    size_t timeInSecondsRDDate = 86400* rDay + 3600 * rHour + 60 * rMinute + rSecond;
 
    // A diferença entre os dois tempos é o atraso de um flight
    return timeInSecondsRDDate - timeInSecondsSDDate;
}

size_t getFlightDelay(Flight* f){
    char* sDepDate = getScheduleDepartureDate(f);
    char* rDepDate = getRealDepartureDate(f); 

    size_t delay = getDelay(sDepDate, rDepDate);

    free(sDepDate);
    free(rDepDate);
    
    return delay;
}

/* ------------------------------------- Getters --------------------------------------- */

int getFlightPassengers(Flight* f) { return g_hash_table_size(f->usersInFlight);}
char* getScheduleDepartureDate(Flight* f) { return strdup(f->scheduleDepartureDate);}
char* getRealDepartureDate(Flight* f) { return strdup(f->realDepartureDate);}
char* getFlightId(Flight* f) { return strdup(f->id);}
char* getFlightAirline(Flight* f) { return strdup(f->airline);}
char* getFlightPlaneModel(Flight* f) { return strdup(f->planeModel);}
char* getFlightOrigin(Flight* f) { return strdup(f->origin);}
char* getFlightDestination(Flight* f) { return strdup(f->destination);}
char* getFlightScheduleArrivalDate(Flight* f) { return strdup(f->scheduleArrivalDate);}

/* -------------------------------------Query10--------------------------------------- */

void addEach(gpointer key, gpointer value, gpointer userData){
    char* userId = (char*) value;
    GHashTable* uniquePassengers = (GHashTable*) userData;
    if (!g_hash_table_contains(uniquePassengers,userId)) g_hash_table_add(uniquePassengers,strdup(userId));
}

void addUniquePassengers(GHashTable* uniquePassengers, Flight* f){
    g_hash_table_foreach(f->usersInFlight,addEach,uniquePassengers);
}
