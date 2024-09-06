#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/parser.h"
#include "../include/userAccess.h"
#include "../include/flightAccess.h"
#include "../include/reservationAccess.h"
#include "../include/passengers.h"
#include "../include/statistics.h"

struct tables{
    UserCTG* userCTG;
    FlightCTG* flightCTG;
    ReservationCTG* reservationCTG;
    Stats* stats;
};

Tables* createTables(){
    return malloc(sizeof(struct tables));
}

Tables* atributeTables(char* inputPath){
    Tables* t = createTables();

    char file[1024];

    sprintf(file,"%s/users.csv",inputPath);
    printf("parsing users.csv\n");
    t->userCTG = newUserCTG(file);
    
    sprintf(file,"%s/flights.csv",inputPath);
    printf("parsing flights.csv\n");
    t->flightCTG = newFlightCTG(file);
    
    sprintf(file,"%s/passengers.csv",inputPath);
    printf("parsing passengers.csv\n");
    hashAtributePassengerFlight(t,file);

    removeInvalidEntries(t->flightCTG,t->userCTG);

    t->stats = newStats();

    sprintf(file,"%s/reservations.csv",inputPath);
    printf("parsing reservations.csv\n");
    t->reservationCTG = newReservationCTG(file,t);

    return t;
}

void destroyTables(Tables* t){
    if(t){
        printf("destroying flights\n");
        if(t->flightCTG) destroyFlightCTG(t->flightCTG);
        t->flightCTG = NULL;

        printf("destroying users\n");
        if(t->userCTG) destroyUserCTG(t->userCTG);
        t->userCTG = NULL;
        
        printf("destroying reservations\n");
        if(t->reservationCTG) destroyReservationCTG(t->reservationCTG);
        t->reservationCTG = NULL;

        if(t->stats) destroyStats(t->stats);
        t->stats = NULL;

        free(t);
        t = NULL;
    }
}

gpointer getUserCTG(Tables* t){
    return t->userCTG;
}

gpointer getFlightCTG(Tables* t){
    return t->flightCTG;
}

gpointer getReservationCTG(Tables* t){
    return t->reservationCTG;
}

gpointer getStats(Tables* t){
    return t->stats;
}