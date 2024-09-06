#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/table.h"
#include "../include/handlequery.h"
#include "../include/interactive.h"

int main(int argc, char** argv){

    /*  agrc == 3 -> modo batch
        argc == 1 -> mode interativo */
    if(argc != 3 && argc != 1){
        printf("Invalid arguments\n");
        return 1;
    }
    
    Tables* tables = NULL;
    
    if(argc == 3){
        tables = atributeTables(argv[1]);
    }

    if (argc == 3){
        interpreter(tables,argv[2],0);
    }

    if (argc == 1){
        char* path = getPathToDataset();
        tables = atributeTables(path);
        free(path);
        readInfo(tables);
    }
    if(tables) destroyTables(tables);

    return 0;
}
