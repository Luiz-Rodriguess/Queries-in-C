#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include "../include/users.h"
#include "../include/util.h"
#include "../include/parser.h"
#include "../include/passengers.h"
#include "../include/reservationAccess.h"
#include "../include/flightAccess.h"
#include "../include/query2.h"

#define CURRENTDATE "2023/10/01" 

struct user{
    char* id;
    char* name;
    char* email;
    char* phoneNumber;
    char* birthDate;
    char sex;
    char* passport;
    char* countryCode;
    char* address;
    char* accountCreation;
    char* payMethod;
    char* accountStatus;
    GHashTable* userFlights;
    GHashTable* userReservations;
};


User* createUser(){
    User* u = (User*) malloc(sizeof (struct user));
    u->id = NULL;
    u->name = NULL;
    u->email = NULL;
    u->phoneNumber = NULL;
    u->birthDate = NULL;
    u->passport = NULL;
    u->countryCode = NULL;
    u->address = NULL;
    u->accountCreation = NULL;
    u->payMethod = NULL;
    u->accountStatus = NULL;
    u->userFlights = g_hash_table_new(g_str_hash,g_str_equal);
    u->userReservations = g_hash_table_new(g_str_hash,g_str_equal);
    return u;
}

void destroyUser(User* u){
    if(u){
        if(u->name) {free(u->name);u->name = NULL;}
        if(u->email) {free(u->email);u->email = NULL;}
        if(u->phoneNumber) {free(u->phoneNumber);u->phoneNumber = NULL;}
        if(u->birthDate) {free(u->birthDate);u->birthDate = NULL;}
        if(u->passport) {free(u->passport);u->passport = NULL;}
        if(u->countryCode) {free(u->countryCode);u->countryCode = NULL;}
        if(u->address) {free(u->address);u->address = NULL;}
        if(u->accountCreation) {free(u->accountCreation);u->accountCreation = NULL;}
        if(u->payMethod) {free(u->payMethod);u->payMethod = NULL;}
        if(u->accountStatus) {free(u->accountStatus);u->accountStatus = NULL;}
        if(u->userFlights) {g_hash_table_destroy(u->userFlights);u->userFlights = NULL;}
        if(u->userReservations) {g_hash_table_destroy(u->userReservations);u->userFlights = NULL;}
        free(u);
        u = NULL;
    }
}

User* getUser(GHashTable* userTable, char* userId){
    return g_hash_table_lookup(userTable, userId);
}

/*  Função que será utilizada na criação da tabela de users para que seja possível 
    chamar as funções de libertação do glib */
void hashDestroyUser(gpointer value){
    destroyUser((User*) value);
}

// função que verifica o email de um User retorna 0 se o email for inválido
int checkEmail(char* email){
    int usernameLen = 0 , domainLen = 0, tldLen = 0, hasAt = 0, hasDot = 0;
    for(unsigned long i=0; i<strlen(email); i++){
        if(email[i] == '@'){
            hasAt++;
            if(i) usernameLen++; // verificar se o @ não é o primeiro caractere
            continue; // evitar de entrar nos outros campos
        }
        if(hasAt && (email[i] == '.')){
            hasDot++;
            domainLen++;
            continue; // evitar de entrar no próximo campo
        }
        if(hasAt && hasDot){
            tldLen++;
        }
    }
    if((usernameLen<1) || (domainLen<1) || (tldLen<2) || (hasAt!=1) || (hasDot!=1)) return 0;
    return 1;
}

User* atributeUser(char* line, FILE* userErrors,GHashTable* userIds){

    char* token;
    char* userId;
    User* u = createUser();
    int field = 0;
    while ((token = strsep(&line, ";"))){
        switch (field) {
            case 0:
                if(strlen(token) < 1){
                    destroyUser(u);
                    return NULL;
                }
                userId = strdup(token); 
                u->id = userId;
                break;
            case 1:
                if(strlen(token) < 1){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->name = strdup(token);
                break;
            case 2:
                if(!checkEmail(token)){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->email = strdup(token);
                break;
            case 3:
                if(strlen(token) < 1){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->phoneNumber = strdup(token);
                break;
            case 4:
                if (!checkDate(token)){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->birthDate = strdup(token);
                break;
            case 5:
                if(strlen(token) < 1){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->sex = token[0];
                break;
            case 6:
                if(strlen(token) < 1){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->passport = strdup(token);
                break;
            case 7:
                if ((strlen(token) != 2) || !isalpha(token[0]) || !isalpha(token[1])){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->countryCode = strdup(token);
                break;
            case 8:
                if(strlen(token) < 1){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->address = strdup(token);
                break;
            case 9:
                if (!checkDateTime(token)){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->accountCreation = strdup(token);
                break;
            case 10:
                if(strlen(token) < 1){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->payMethod = strdup(token);
                break;
            case 11:
                token = removeEnter(token);
                if((strcasecmp("active",token) != 0) && (strcasecmp("inactive",token) != 0)){
                    destroyUser(u);
                    free(userId);
                    return NULL;
                }
                u->accountStatus = strdup(token);
                break;
        }
        field++;
    }
    if (!endAfterStartDate(u->accountCreation,u->birthDate)){
        destroyUser(u);
        free(userId);
        return NULL;
    }
    g_hash_table_add(userIds,userId);
    return u;
}

void fillUserTable(GHashTable* userTable, char* line, FILE* userErrors,gpointer userData){
    char* error = strdup(line);
    GHashTable* userIds = (GHashTable*) userData;
    User* u = atributeUser(line,userErrors,userIds);
    if (u) g_hash_table_insert(userTable,(u->id),u);
    else fputs(error,userErrors);
    free(error);
}

struct userAux{
    GHashTable* userTable;
    GHashTable* userIds;
};

UserAux* hashAtributeUser(char* userCSV){

    GHashTable* userTable = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,hashDestroyUser);
    GHashTable* userIds = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,free);

    FILE* userErrors = fopen("Resultados/users_errors.csv", "w");

    parse(userCSV,userTable,fillUserTable,userErrors,userIds);

    fclose(userErrors);

    UserAux* aux = (UserAux*) malloc(sizeof(struct userAux));
    aux->userTable = userTable;
    aux->userIds = userIds;

    return aux;
}

GHashTable* getUserTableAux(UserAux* aux) { return aux->userTable;}

GHashTable* getUserIdsAux(UserAux* aux) { return aux->userIds;}

void destroyUserTable(GHashTable* userTable){ g_hash_table_destroy(userTable);}

void destroyUserIds(GHashTable* userIds){ g_hash_table_destroy(userIds);}

void addReservationToReservationSet(GHashTable* userTable,char* userId, char* reservationId){
    User* u = g_hash_table_lookup(userTable,userId);
    if (u) g_hash_table_add(u->userReservations,reservationId);
}

void addFlightToFlightSet(GHashTable* userTable,char* userId, char* flightId){
    User* u = g_hash_table_lookup(userTable,userId);
    if (u) g_hash_table_add(u->userFlights,flightId);
}

int isActive(User* u){
    if (u) return (strcasecmp(u->accountStatus,"active") == 0);
    return 0;
}

GHashTable* getuserReservations(GHashTable* userTable,char* userId){
    User* u = g_hash_table_lookup(userTable,userId);
    return u->userReservations;
}

int getAge(char* uDate, char* cDate){
    int uYear, uMonth, uDay;
    int cYear, cMonth, cDay;

    sscanf(uDate,"%d/%d/%d",&uYear,&uMonth,&uDay);
    sscanf(cDate,"%d/%d/%d",&cYear,&cMonth,&cDay);
    
    int age = cYear - uYear;
    if (cMonth < uMonth) age--;
    else if (cMonth == uMonth && cDay < uDay) age--;
    
    return age;
}

char* getUserBirthDate(User* u){ return strdup(u->birthDate);}

int getUserAge(User* u){
    char* uDate = getUserBirthDate(u);
    char* cDate = strdup(CURRENTDATE);

    int age = getAge(uDate, cDate);

    free(uDate);
    free(cDate);

    return age;
}

char* getUserCreationDate(User* u){ return strdup(u->accountCreation);}
char* getUserId(User* u){ return strdup(u->id);}
char* getUserName(User* u) { return strdup(u->name);}
char getUserSex(User* u) { return u->sex ;}
char* getUserCountryCode(User* u) { return strdup(u->countryCode);}
char* getUserPassport(User* u) { return strdup(u->passport);}
int getNumberOfFlights(User* u) { return g_hash_table_size(u->userFlights);}
int getNumberOfReservations(User* u) { return g_hash_table_size(u->userReservations);}
double getUserTotalSpent(User* u,Tables* t) {  return calculateUserTotalSpent(t,u->userReservations);}

/* -------------------------------------Query2--------------------------------------- */

int getFlightDates(User* u,Tables* t,int flag,GTree* tree){
    GHashTableIter iter;
    gpointer key,value;

    GHashTable* userFlights = u->userFlights;

    if (!userFlights) return 0;
    
    if (userFlights){
        g_hash_table_iter_init(&iter,userFlights);
        while (g_hash_table_iter_next(&iter,&key,&value)){
            char* flightId = (char*) value;
            char* sDDate = getSDDateCTG(getFlightCTG(t),flightId);

            Q2AUX* q = newQ2(sDDate,flightId,"flight",flag);
            g_tree_insert(tree,q,NULL);
        }
    }
    return 1;
}

int userReservationDates(User* u,Tables* t,int flag,GTree* tree){
    GHashTableIter iter;
    gpointer key, value;

    GHashTable* userReservations = u->userReservations; 

    if (!userReservations) return 0;

    g_hash_table_iter_init(&iter, userReservations);

    if (userReservations){
        while(g_hash_table_iter_next(&iter,&key,&value)){
            char* reservationId = (char*) value;
            char* beginDate = getReservationBeginDateCTG(getReservationCTG(t),reservationId);
            char* beginDateTime = addTimeToDate(beginDate);
            
            Q2AUX* q = newQ2(beginDateTime,reservationId,"reservation",flag);
            free(beginDate);
            g_tree_insert(tree,q,NULL);
        }
    }
    return 1;
}

/* -------------------------------------Query9--------------------------------------- */

int userHasPrefix(User* u, char* prefix){
    char* userName = u->name;
    if (strlen(prefix) > strlen(userName))return 0;

    for(unsigned long i=0;i< strlen(prefix);i++){
        if(prefix[i] != userName[i]) return 0;
    }

    return 1;
}
