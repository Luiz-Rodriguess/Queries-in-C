#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/passengers.h"
#include "../include/util.h"
#include "../include/parser.h"
#include "../include/table.h"
#include "../include/flightAccess.h"
#include "../include/userAccess.h"

void fillPassengerFlightTable(GHashTable* passengerFlightTable, char* line, FILE* passErrors,gpointer tables){
    Tables* t = (Tables*) tables;

    char* error = strdup(line);
    char* flightId = NULL;
    char* userId = NULL;
    int field = 0;
    char* token;
    while ((token = strsep(&line, ";"))){
        switch (field) {
            case 0:
                flightId = token;
                break;
            case 1:
                userId = removeEnter(token);
                break;
        }
        field++;
    }
    if(strlen(flightId)<1 || strlen(userId)<1){
        fputs(error,passErrors);
    }
    else if (!existingUser(getUserCTG(t),userId) || !existingFlight(getFlightCTG(t),flightId)){
        fputs(error,passErrors);
    }else{
        FlightCTG* flightCTG = getFlightCTG(t);
        UserCTG* userCTG = getUserCTG(t);
        addUserToFlightCTG(flightCTG,getUserIdCTG(userCTG,userId),getFlightIdCTG(flightCTG,flightId));
    }
    free(error);
}

void hashAtributePassengerFlight(Tables* t,char* passengerCSV){

    FILE* passErrors = fopen("Resultados/passengers_errors.csv", "w");

    parse(passengerCSV,NULL,fillPassengerFlightTable,passErrors,t);
    
    fclose(passErrors);
}

