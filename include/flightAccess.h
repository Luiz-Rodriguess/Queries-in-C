#ifndef flightaccess
#define flightaccess
#include "../include/userAccess.h"
#include "../include/statistics.h"
#include "../include/query1.h"

/* Módulo de acesso aos flights, presente para manter o 
   encapsulamento e não permitir acesso direto às estruturas */

/* Estrutura que armazena as tabelas e que permite o acesso aos flights */
typedef struct flightCTG FlightCTG;

/* Função que verifica se um voo existe */
gboolean existingFlight(FlightCTG* ctg,char* flightId);

/* Função que cria uma nova estrutura FlightCTG */
FlightCTG* newFlightCTG(char* flightCSV);

/* Função que liberta a memória associada a uma FlightCTG */
void destroyFlightCTG(FlightCTG* ctg);

/*  Função utilizada na validação dos datasets que remove os voos inválidos por excesso de passageirots
    Também envia informações para um UserCTG para inserir os voos de um user */
void removeInvalidEntries(FlightCTG* flightCTG, UserCTG* userCTG);

/* Função utilizada para inserir os users lidos do passenger.csv */
void addUserToFlightCTG(FlightCTG* ctg, char* userId, char* flightId);

/* Função utilizada para obter um id de um voo, utilizada para não ter cópias em excesso de um mesmo id */
char* getFlightIdCTG(FlightCTG* ctg, char* flightId);

/* Função para obter o resultado da query 1 para flights */
Q1AUXF* flightInfoQ1CTG(gpointer tables, char* flightId);

/* Função para obter o schedule departure date de um voo à partir do FlightCTG */
char* getSDDateCTG(FlightCTG* ctg, char* flightId);

/*  Função para obter as informações necessárias para a query 5
    necessita da GTree que deseja ser inserido */
int listFLightsOrigin(FlightCTG* ctg,char* airportName,char* beginDate,char* endDate,GTree* tree);

/*  Função para obter as informações necessárias para a query 6
    necessita da GTree que deseja ser inserido */
void passengersOnYear(FlightCTG* ctg, char* year,GTree* tree);

/*  Função para obter as informações necessárias para a query 5
    necessita da GTree que deseja ser inserido */
void getMedianDelays(FlightCTG* ctg,GTree* tree);

/*  Função para obter as informações necessárias para a query 5
    necessita da GHashTable que deseja ser inserido */
void flightStatsQ10(FlightCTG* ctg,GHashTable* tabelaQ10,char* yearOpt,char* monthOpt);

#endif