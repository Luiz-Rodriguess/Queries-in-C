#ifndef useraccess
#define useraccess

#include "../include/query1.h"

/* Módulo de acesso aos users, presente para manter o 
   encapsulamento e não permitir acesso direto às estruturas */

/* Estrutura que armazena as tabelas referentes a um User */
typedef struct userCTG UserCTG;

/* Função que cria um novo UserCTG */
UserCTG* newUserCTG(char* userCSV);

/* Função que verifica se um User existe */
gboolean existingUser(UserCTG* ctg,char* userId);

/* Função que verifica se um User está ativo */
int isActiveUserCTG(UserCTG* ctg,char* userId);

/* Função que liberta a memória associada a um UserCTG */
void destroyUserCTG(UserCTG* ctg);

/* Função utilizada para obter um id de um user, utilizada para não ter cópias em excesso de um mesmo id */
char* getUserIdCTG(UserCTG* ctg,char* userId);

/* Função que adiciona um flight ao set de um user é feito depois da validação total dos flights para simplificar */
void addFlightToUser(UserCTG* ctg, char* userId, char* flightId);

/* Função que adiciona uma resera ao set de um user */
void addReservationToUser(UserCTG* ctg, char* userId,char* reservationId);

/* Função que retorna as informações de um user para a query 1 */
Q1AUXU* userInfoQ1CTG(gpointer tables,char* userId);

/* Função que obtem as informações de um flight para a query 2 */
int getFlightDatesCTG(UserCTG* ctg, char* userId,Tables* t,int flag, GTree* tree);

/* Funçao que obtem as informações de uma reservation para a query 2 */
int userReservationDatesCTG(UserCTG* ctg, char* userId, Tables* t, int flag, GTree* tree);

/* Função que adiciona os users que contém um prefixo no seu nome à GTree recebida */
void usersWithPrefix(UserCTG* ctg, char* prefix, GTree* tree);

/* Função que insere os novos users na tabela auxiliar da query 10 */
void newUsers(UserCTG* ctg,GHashTable* tabelaQ10,char* yearOpt, char* monthOpt);

#endif 