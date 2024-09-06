#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/interactive.h"
#include "../include/util.h"
#include "../include/table.h"
#include "../include/handlequery.h"

char* getPathToDataset(){
    char* line = NULL;
    size_t len = 0;

    while (!line){
        printf("\033c");
        printf("Insert path to dataset:\n");
        if (getline(&line,&len,stdin) != -1){
            if (strcmp(line,"\n") == 0){ 
                if(line) free(line);
                return strdup("dataset/data");
            }
            return removeEnter(line);
        }
    }
    return NULL;
}

void readInfo(Tables* t){
    char* line = NULL;
    size_t len = 0;
    
    int keepPrinting = 1;
    while (keepPrinting){
        printf("\033c");
        printf("Insert query number and arguments\nLeave blank to quit program:\n");
        if (getline(&line,&len,stdin) == -1 || strcmp(line,"\n") == 0) keepPrinting = 0;
        else{
            handle(t,line,NULL);
        }
    }
        
    if (line) free(line);
}

