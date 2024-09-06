#ifndef flights
#define flights
#include <glib.h>
#include "../include/table.h"
#include "../include/statistics.h"
#include "../include/userAccess.h"

/* Módulo que trata da lógica referente a um Flight */

/* Estrutura que guarda todas as informações contidas no flights.csv */
typedef struct flight Flight;

/* Estrutura auxiliar para enviar as tabelas para um FlightCTG */
typedef struct flightAux FlightAux;

/* Função utilizada para atribuir os voos */
FlightAux* hashAtributeFlight(char* flightSpreadsheet);

/* Função que retorna para o FLightCTG a tabela de Flights */
GHashTable* getFlightTableAux(FlightAux* aux);

/* Função que retorna para o FlightCTG o set que contém os ids válidos */
GHashTable* getFlightIdsAux(FlightAux* aux);

/* Função que permite libertar a memória associada à tabela de Flights */
void destroyFlightTable(GHashTable* flightTable);

/* Função que permite libertar a memória associada ao set de ids válidos */
void destroyFlightIds(GHashTable* flightIds);

/* Função utilizada para remover os voos em que o número de passageiros excede o totalSeats definido */
void removeExceededTotalSeats(GHashTable* flightTable, GHashTable* flightIds,GHashTable* airportFlights,UserCTG* userCTG);

/* Função que retorna uma cópia do schedule departure date de um flight */
char* getScheduleDepartureDate(Flight* f);

/* Função que retorna uma cópia do real departure date de um flight */
char* getRealDepartureDate(Flight* f);

/* Função que retorna uma cópia do id de um flight */
char* getFlightId(Flight* f);

/* Função que retorna o total de passageiros de um flight */
int getFlightPassengers(Flight* f);

/* Função que retorna uma cópia da airline de um flight */
char* getFlightAirline(Flight* f);

/* Função que retorna uma cópia do plane model de um flight */
char* getFlightPlaneModel(Flight* f);

/* Função que retorna uma cópia do aeroporto de origem de um flight */
char* getFlightOrigin(Flight* f);

/* Função que retorna uma cópia do aeroporto de destino de um flight */
char* getFlightDestination(Flight* f);

/* Função que retorna uma cópia do schedule arrival date de um flight */
char* getFlightScheduleArrivalDate(Flight* f);

/*  função que calcula o atraso de um Flight */
size_t getFlightDelay(Flight* f);

/* Função que adiciona os passageiros ao set de um Flight */
void addUserToUserSet(GHashTable* flightTable,char* userId, char* flightId);

/* Função auxiliar para a query 10 que adiciona os passageiros distintos */
void addUniquePassengers(GHashTable* uniquePassengers, Flight* f);

#endif
