#ifndef TABLE
#define TABLE

#include <glib.h>

/* Estrutura que guarda os CTGs e a Stats */
typedef struct tables Tables;

/* Função que cria a estrutura Tables e cria cada CTG e o stats */
Tables* atributeTables(char* inputPath);

/* Função que liberta a memória associada a estrutura Tables */
void destroyTables(Tables* t);

/* Função que retorna o UserCTG presente na estrutura */
gpointer getUserCTG(Tables* t);

/* Função que retorna o FlightCTG presente na estrutura */
gpointer getFlightCTG(Tables* t);

/* Função que retorna o ReservationCTG presente na estrutura */
gpointer getReservationCTG(Tables* t);

/* Funçaõ que retorna o Stats presente na estrutura */
gpointer getStats(Tables* t);

#endif