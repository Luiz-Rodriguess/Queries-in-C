#ifndef query1
#define query1
#include "../include/table.h"

/* Função que recebe uma linha com as informações referentes a query 1, separa os argumentos e incia a execução */
void formatQuery1(Tables* t,char* line,int format,FILE* output);

/* Estrutura auxiliar para armazenar as informações relevantes ao User */
typedef struct query1AuxUser Q1AUXU;

/* Função que cria uma nova instância da Q1AUXU */
Q1AUXU* newQ1U(char* name,char sex,int userAge,char* counrtyCode,char* passport,int numberOfFlights,int numberOfReservations, double totalPrice);

/* Estrutura auxiliar para armazenar as informações relevantes ao Flight */
typedef struct query1AuxFlight Q1AUXF;

/* Função que cria uma nova instância da Q1AUXF */
Q1AUXF* newQ1F(char* airline,char* planeModel,char* origin,char* destination,char* sDDate,char* sADate,int totalPassengers,size_t delay);

/* Estrutura auxiliar para armazenar as informações relevantes a Reservation */
typedef struct query1AuxReservation Q1AUXR;

/* Função que cria uma nova instância da Q1AUXR */
Q1AUXR* newQ1R(char* hotelId,char* hotelName,int hotelStars,char* beginDate,char* endDate,char* includesBreakfast,int totalNights,double totalPrice);

#endif