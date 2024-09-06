#ifndef users
#define users
#include <glib.h>
#include <stdio.h>
#include "../include/table.h"

/* Estrutura que guarda as informações contidas no users.csv */
typedef struct user User;

/* Estrutura auxiliar que guarda as tabelas que serão enviadas para o UserCTG */
typedef struct userAux UserAux;

/* Função utilizada para validar as entradas do users.csv e inserir nas tabelas */
UserAux* hashAtributeUser(char* userCSV);

/* Função que retorna a tabela de users para o UserCTG */
GHashTable* getUserTableAux(UserAux* aux);

/* Função que retona a tabela de ids válidos para a UserCTG */
GHashTable* getUserIdsAux(UserAux* aux);

/* Função que liberta a memória associada à tabela de users */
void destroyUserTable(GHashTable* userTable);

/* Função que liberta a memória associada à tabela de ids de user */
void destroyUserIds(GHashTable* userIds);

/* Funão que verifica se um user está ativo */
int isActive(User* u);

/* Função que retorna um id de um user */
char* getUserId(User* u);

/* Função quer retorna o user com aquele id  */
User* getUser(GHashTable* userTable, char* userId);

// query1 //

/* Função que retorna a idade de um user */
int getUserAge(User* u);

/* Função que retorna uma cópia do nome de um user */
char* getUserName(User* u);

/* Função que retorna o sexo de um user */
char getUserSex(User* u);

/* Função que retorna uma cópia da creation date de um user */
char* getUserCreationDate(User* u);

/* Função que retorna uma cópia do country code de um user */
char* getUserCountryCode(User* u);

/* Função que retorna uma cópia do passport de um user */
char* getUserPassport(User* u);

/* Função que retorna o número de voos de um user */
int getNumberOfFlights(User* u);

/* Função que retorna o número de reservas de um user */
int getNumberOfReservations(User* u);
 
/* Função que retorna o total gasto em reservas de um user */
double getUserTotalSpent(User* u,Tables* t);

/* Função que adiciona um voo ao set do user */
void addFlightToFlightSet(GHashTable* userTable,char* userId, char* flightId);

/* Função que adiciona uma reserva ao set do user */
void addReservationToReservationSet(GHashTable* userTable,char* userId, char* reservationId);

/* Função quer retorna a tabela de reservas de um user */
GHashTable* getuserReservations(GHashTable* userTable,char* userId);

/* Função que obtem as informações para a query 2 sobre os flights */
int getFlightDates(User* u,Tables* t,int flag,GTree* tree);

/* Função que obtem as informações para a query 2 sobre as reservatrions */
int userReservationDates(User* u,Tables* t,int flag,GTree* tree);

/* Função que verifica se o nome de um user tem aquele prefixo */
int userHasPrefix(User* u, char* prefix);

#endif
