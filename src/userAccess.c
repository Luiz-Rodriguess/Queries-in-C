#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/userAccess.h"
#include "../include/users.h"
#include "../include/util.h"
#include "../include/table.h"
#include "../include/query1.h"
#include "../include/query9.h"
#include "../include/query10.h"

struct userCTG{
    GHashTable* userTable;
    GHashTable* userIds;   
};

UserCTG* newUserCTG(char* userCSV){
    UserCTG* ctg = (UserCTG*) malloc(sizeof(struct userCTG));
    UserAux* aux = hashAtributeUser(userCSV);
    ctg->userTable = getUserTableAux(aux);
    ctg->userIds = getUserIdsAux(aux);
    if(aux) free(aux);
    return ctg;
}

void destroyUserCTG(UserCTG* ctg){
    if (ctg){
        if (ctg->userTable) destroyUserTable(ctg->userTable);
        if (ctg->userIds) destroyUserIds(ctg->userIds);
        free(ctg);
    }
}

gboolean existingUser(UserCTG* ctg,char* userId){
    return g_hash_table_contains(ctg->userIds,userId);
}

int isActiveUserCTG(UserCTG* ctg,char* userId){
    if (existingUser(ctg,userId)){
        User* u = g_hash_table_lookup(ctg->userTable,userId);
        return isActive(u);
    }
    return 0;
}

char* getUserIdCTG(UserCTG* ctg,char* userId){
    return (char*) g_hash_table_lookup(ctg->userIds,userId);
}

void addFlightToUser(UserCTG* ctg, char* userId, char* flightId){
    addFlightToFlightSet(ctg->userTable,userId,flightId);
}

void addReservationToUser(UserCTG* ctg, char* userId,char* reservationId){
    addReservationToReservationSet(ctg->userTable,userId,reservationId);
}

Q1AUXU* userInfoQ1CTG(gpointer tables,char* userId){
    Tables* t = (Tables*) tables;
    UserCTG* ctg = getUserCTG(t);
    User* u = getUser(ctg->userTable,userId);
    char* name  = getUserName(u);
    char sex = getUserSex(u);
    int userAge = getUserAge(u);
    char* countryCode = getUserCountryCode(u);
    char* passport = getUserPassport(u);
    int numberOfFlights = getNumberOfFlights(u);
    int numberOfReservations = getNumberOfReservations(u);
    double totalSpent = getUserTotalSpent(u,t);
    return newQ1U(name,sex,userAge,countryCode,passport,numberOfFlights,numberOfReservations,totalSpent);
}

int getFlightDatesCTG(UserCTG* ctg, char* userId,Tables* t,int flag, GTree* tree){
    User* u = g_hash_table_lookup(ctg->userTable,userId);
    return getFlightDates(u,t,flag,tree);
}

int userReservationDatesCTG(UserCTG* ctg, char* userId, Tables* t, int flag, GTree* tree){
    User* u = g_hash_table_lookup(ctg->userTable,userId);
    return userReservationDates(u,t,flag,tree);
}

void usersWithPrefix(UserCTG* ctg, char* prefix, GTree* tree){

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter,ctg->userTable);
    
    while(g_hash_table_iter_next(&iter,&key,&value)){
        User* u = (User*) value;
        if(isActive(u) && userHasPrefix(u,prefix)){
            Q9AUX* q = newQ9(getUserId(u),getUserName(u));
            g_tree_insert(tree,q,NULL);
        }
    }

}

void newUsers(UserCTG* ctg,GHashTable* tabelaQ10,char* yearOpt, char* monthOpt){
    int flag = 1;
    if(monthOpt) flag = 3; // Ano e mês como opção na query
    else if (yearOpt) flag = 2; // Ano como opção na query
    
    GHashTableIter iter;
    gpointer key,value;

    g_hash_table_iter_init(&iter,ctg->userTable);

    while(g_hash_table_iter_next(&iter,&key,&value)){
        User* u = (User*) value;
        char* creationDate = getUserCreationDate(u);
        switch (flag) {
            // caso em que não foi especificado nem ano nem mês
            case 1:{
                char* year = getYearFromDate(creationDate);
                Q10AUX* q = g_hash_table_lookup(tabelaQ10,year);

                if(q == NULL){    
                    q = newQ10(year);
                    g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                }

                incQ10Users(q);
                if(year) free(year);
                break;
            }
            case 2:{
                // caso em que o ano é especificado
                if(isInOpt(creationDate,yearOpt)){
                    char* month = getMonthFromDate(creationDate);
                    Q10AUX* q = g_hash_table_lookup(tabelaQ10,month);

                    if(q == NULL){
                        q = newQ10(month);
                        g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                    }

                    incQ10Users(q);
                    if(month) free(month);
                }
                break;
            }
            case 3:{
                // caso em que o ano e o mês são especificados 
                char opt [strlen(yearOpt) + strlen(monthOpt) + 2];
                sprintf(opt,"%s/%s",yearOpt,monthOpt);
                
                if(isInOpt(creationDate,opt)){
                    char* day = getDayFromDate(creationDate);
                    Q10AUX* q = g_hash_table_lookup(tabelaQ10,day);

                    if(q == NULL){
                        q = newQ10(day);
                        g_hash_table_insert(tabelaQ10,getYMDQ10(q),q);
                    }

                    incQ10Users(q);
                    if(day) free(day);
                }
            }
        }
        if (creationDate) free(creationDate);
    }
}