#ifndef reservationaccess
#define reservationaccess

#include "../include/query1.h"

/* Módulo de acesso as reservations, presente para manter o 
   encapsulamento e não permitir acesso direto às estruturas */

/* Estrutura que armazena as tabelas e que permite o acesso aos reservations */
typedef struct reservationCTG ReservationCTG;

/* Função que cria uma nova estrutura FlightCTG */
ReservationCTG* newReservationCTG(char* reservationCSV,gpointer userData);

/* Função que liberta a memória associada à uma ReservationCTG */
void destroyReservationCTG(ReservationCTG* ctg);

/* Função que calcula o total gasto por um user com base nas suas reservas */
double calculateUserTotalSpent(gpointer tables,GHashTable* userReservations);

/* Função que verifica se uma Reservation existe */
gboolean existsReservation(ReservationCTG* ctg, char* reservationId);

/* Funçaõ que retorna as informações necessárias para a query 1 */
Q1AUXR* reservationInfoQ1CTG(gpointer tables,char* reservationId);

/* Funçaõ que retorna o begin date de uma reserva à partir do CTG */
char* getReservationBeginDateCTG(ReservationCTG* ctg, char* reservationId);

/* Função que retorna a classificação média de um hotel para query 3 */
double getClassMedia(ReservationCTG* ctg,char* hotelId);

/* Função que calcula as informações para a query 4 */
int listHotelRes(ReservationCTG* ctg,char* hotelId,GTree* tree);

/* Função que calcula o lucro de um hotel para a query 8 */
int getRevenueHotel(ReservationCTG* ctg,char* hotelId,char* beginDate,char* endDate);

/* Função que insere as informações para a tabela auxiliar da query 10 */
void numberOfReservations(ReservationCTG* ctg,GHashTable* tabelaQ10,char* yearOpt, char* monthOpt);

#endif