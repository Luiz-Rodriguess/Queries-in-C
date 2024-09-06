#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/util.h"

/* Módulo de utilidades */

char* removeEnter(char* line){ 
    int len = strlen(line); 
    if (line[len - 1] == '\n'){
        len--;
        line[len] = '\0';
    }
    return line;
}

int checkDate(char* date){
    int year, month, day;
    int res = sscanf(date,"%d/%d/%d",&year,&month,&day);
    if (res<3 || month<1 || month>12 || day<1 || day>31){
        return 0;
    }
    return 1;
}

int checkDateTime(char* dateTime){
    int year, month, day, hour, minute, second;

    int res = sscanf(dateTime,"%d/%d/%d %d:%d:%d",&year,&month,&day,&hour,&minute,&second);
    
    if (res<6 || month<1 || month>12 || day<1 || day>31 || hour<0 || hour>23 || minute<0 || minute>59 || second<0 || second>59){
        return 0;
    }
    return 1;
}

int endAfterStartDate(char* end,char* start){ 
    if (strcmp(end,start) == 0) return 1;
    
    int sYear, sMonth, sDay;
    int eYear, eMonth, eDay;

    sscanf(start,"%d/%d/%d",&sYear,&sMonth,&sDay);
    sscanf(end,"%d/%d/%d",&eYear,&eMonth,&eDay);
    //os casos a seguir servem como filtros
    if(eYear > sYear) return 1; //só passam os anos menores ou iguais
    if(eYear < sYear) return 0; //só passam os anos iguais
    if(eMonth > sMonth) return 1; // só passam os meses menores ou iguais
    if(eMonth < sMonth) return 0; // só passam os meses iguais
    if(eDay >= sDay) return 1; // só passam os dias menores
    return 0;
}

int endAfterStartDateTime(char* end,char* start){
    if (strcmp(end,start) == 0) return 1;    
    int sYear, sMonth, sDay, sHour, sMinute, sSecond;
    int eYear, eMonth, eDay, eHour, eMinute, eSecond;

    sscanf(start,"%d/%d/%d %d:%d:%d",&sYear,&sMonth,&sDay,&sHour,&sMinute,&sSecond);
    sscanf(end,"%d/%d/%d %d:%d:%d",&eYear,&eMonth,&eDay,&eHour,&eMinute,&eSecond);
  
    //os casos a seguir servem como filtros
    if(eYear > sYear) return 1; //só passam os anos menores ou iguais
    if(eYear < sYear) return 0; //só passam os anos iguais
    if(eMonth > sMonth) return 1; // só passam os meses menores ou iguais
    if(eMonth < sMonth) return 0; // só passam os meses iguais
    if(eDay > sDay) return 1; // só passam os dias menores ou iguias
    if(eDay < sDay) return 0; //só passam os dias iguais
    if(eHour > sHour) return 1; // só passam as horas maiors ou iguais
    if(eHour < sHour) return 0; // só passam as horas iguais
    if(eMinute > sMinute) return 1; // só passam os minutos menores ou iguais
    if(eMinute < sMinute) return 0; // só passam os minutos iguais
    if(eSecond >= sSecond) return 1; // só passam os segundos menores

    return 0;
}

struct fileAndCounter{
    int count;
    int max;
    int format;
    FILE* output;
};

FAC* newFAC(FILE* output,int count,int max,int format){
    FAC* f = (FAC*) malloc(sizeof(struct fileAndCounter));
    f->count = count;
    f->output = output;
    f->max = max; 
    f->format = format;
    return f;
}

void destroyFAC(FAC* f){ if(f) free(f);}

void incFAC(FAC* f){ f->count++;}

FILE* getFACOutput(FAC* f){ return f->output;}

int getFACCount(FAC* f){ return f->count;}

int getFACMax(FAC* f){ return f->max;}

int getFACFormat(FAC* f){ return f->format;}

char* getYearFromDate(char* date){
    char year[5];
    strncpy(year,date,4);
    year[4] = '\0';
    return strdup(year);
}

char* getMonthFromDate(char* date){
    char month[3];
    strncpy(month,date+5,2);
    month[2] = '\0';
    return strdup(month);
}

char* getDayFromDate(char* date){
    char day[3];
    strncpy(day,date+8,2);
    day[2] = '\0';
    return strdup(day);
}

char* getDateFromDateTime(char* dateTime){
    char date[11];
    strncpy(date,dateTime,10);
    date[10] = '\0';
    return strdup(date);
}

char* addTimeToDate(char* date){
    char* time = "00:00:00";
    size_t len = strlen(date) + strlen(time) + 2;
    char dateTime[len];
    snprintf(dateTime,len,"%s %s",date,time);
    return (strdup(dateTime));
}

int isInOpt(char* date,char* opt){
    if (strncmp(date,opt,strlen(opt)) == 0) return 1;
    return 0;
}

int checkInt(char* line){
     // flag para pararmos o ciclo quando encontrarmos algo que não seja um número
    int flag = 1;
    for(unsigned long i=0;flag && i<strlen(line); i++){
        if(!isdigit(line[i])) flag = 0;
    }
    return flag;
}


void substituiSubstrings(char *str,char *subStr,char *substituicao){
    int strLen = strlen(str);
    int subStrLen = strlen(subStr);
    int substituicaoLen = strlen(substituicao);

    for (int i = 0; i <= strLen - subStrLen; i++) {
        int j;

        for (j = 0; j < subStrLen; j++) {
            if (str[i + j] != subStr[j]) {
                break;
            }
        }

        if (j == subStrLen) {
            
            memmove(&str[i + substituicaoLen], &str[i + subStrLen], strLen - (i + subStrLen) + 1);
            memcpy(&str[i], substituicao, substituicaoLen);
            i += substituicaoLen - 1;
            strLen = strlen(str);
        }
    }
}

char* normalizeString(char* str){
    char* newStr = strdup(str);
    substituiSubstrings(newStr,"á", "a");
    substituiSubstrings(newStr,"ã", "a");
    substituiSubstrings(newStr,"â", "a");
    substituiSubstrings(newStr,"Á", "A");
    substituiSubstrings(newStr,"Ã", "A");
    substituiSubstrings(newStr,"Â", "A");
    substituiSubstrings(newStr,"é", "e");
    substituiSubstrings(newStr,"ê", "e");
    substituiSubstrings(newStr,"É", "E");
    substituiSubstrings(newStr,"Ê", "E");
    substituiSubstrings(newStr,"í", "i");
    substituiSubstrings(newStr,"Í", "I");
    substituiSubstrings(newStr,"ó", "o");
    substituiSubstrings(newStr,"Ó", "O");
    substituiSubstrings(newStr,"ú", "u");
    substituiSubstrings(newStr,"Ú", "u");
    substituiSubstrings(newStr,"ç", "c");
    substituiSubstrings(newStr,"Ç", "C");
    substituiSubstrings(newStr,"-", "");
    substituiSubstrings(newStr," ", "");
    return newStr;
}

void destroyAux(gpointer value){
    GHashTable* aux = (GHashTable*) value;
    g_hash_table_destroy(aux);
}

void red(){
    printf("\033[0;31m");
}

void reset(){
    printf("\033[0m");
}

void green(){
    printf("\033[0;32m");
}

void yellow(){
    printf("\033[0;33m");
}
