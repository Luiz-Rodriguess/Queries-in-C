#ifndef reservations
#define reservations
#include <glib.h>
#include "../include/table.h"
#include "../include/users.h"

/* Estrutura que guarda as informações do reservations.csv */
typedef struct reservation Reservation;

/* Estrutura auxiliar para criar as tabelas da ReservationCTG */
typedef struct reservationAux ReservationAux;

/* Função que vai ler o ficheiro para validar e inserir as informações nas tabelas */
ReservationAux* hashAtributeReservation(Tables* t,char* reservationCSV);

/* Função para passar a tabela de reservas para o ReservationCTG */
GHashTable* getReservationTable(ReservationAux* aux);

/* Função para passar a tabela que contém as reservas de cada hotel para o ReservationCTG */
GHashTable* getHotelReservations(ReservationAux* aux);

/* Função que liberta a memória associada à tabela de reservas */
void destroyReservationTable(GHashTable* reservationTable);

/* Função que obtém as informações das reservas de um User para a query 1 */
double userResInfoQ1(GHashTable* reservationTable,GHashTable* userReservations);

/* Função que retorna uma cópia do begin date de uma Reservation */
char* getBeginDate(Reservation* r);

/* Função que retorna o número de noites de uma Reservation */
int getReservationNights(Reservation* r);

/* Função que retorna uma cópia do hotel id de uma Reservation */
char* getReservationHotelId(Reservation* r);

/* Função que retorna uma cópia do hotel name de uma Reservation */
char* getReservationHotelName(Reservation* r);

/* Função que retorna o número de estrelas de uma Reservation */
int getReservationHotelStars(Reservation* r);

/* Função que retorna uma cópia do end date de uma Reservation */
char* getReservationEndDate(Reservation* r);

/* Função que retorna uma cópia do hotel name de uma Reservation */
char* getReservationBeginDate(Reservation* r);

/* Função que retorna uma cópia do includes breakfast de uma Reservation */
char* getReservationIncludesBreakfast(Reservation* r);

/* Função que retorna o preço de uma Reservation */
double getTotalPriceReservation(Reservation* r,int totalNights);

/* Função que retorna o rating como um número de uma Reservation */
double getReservationRating(Reservation* r);

/* Função que retorna a cópia do rating de uma Reservation */
char* getReservationRatingAsString(Reservation* r);

/* Função que retorna a cópia do id de uma Reservation */
char* getReservationId(Reservation* r);

/* Função que retorna a cópia do user id de uma Reservation */
char* getReservationUserId(Reservation* r);

/* Função que verifica se uma Reservation está entre duas datas */
int doesReservationIntersect(Reservation* r,char* beginDate, char* endDate);

/* Função que retorna o valor necessário para a quert 8 */
int getTotalPriceQ8(Reservation* r,char* beginDate, char* endDate);

#endif
