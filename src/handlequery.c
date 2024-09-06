#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <time.h>
#include "../include/handlequery.h"
#include "../include/query1.h"
#include "../include/query2.h"
#include "../include/query3.h"
#include "../include/query4.h"
#include "../include/query5.h"
#include "../include/query6.h"
#include "../include/query7.h"
#include "../include/query8.h"
#include "../include/query9.h"
#include "../include/query10.h"
#include "../include/table.h"
#include "../include/util.h"

#define LINESIZE 1024
#define FILENAMESIZE 128

int getFormat(char* line,int queryNumber){
    if (queryNumber == 10){
        return line[2] == 'F';
    }
    return line[1] == 'F';
}

int getQueryNumber(char* line){
    int queryNumber;
    sscanf(line,"%d",&queryNumber);
    return queryNumber;
}

void interpreter(Tables* t,char* input,int opt){
    printf("Running queries\n");

    FILE* queryFile = fopen(input,"r");

    if (!queryFile) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    int commandNumber = 1;

    char fileName[FILENAMESIZE];

    char line[LINESIZE];
    
    while(fgets(line,LINESIZE,queryFile)){
        struct timespec start, end;
        double elapsed;
        clock_gettime(CLOCK_REALTIME,&start);

        snprintf(fileName,FILENAMESIZE,"Resultados/command%d_output.txt",commandNumber);
        FILE* output = fopen(fileName,"w");

        int queryNumber = getQueryNumber(line);

        handle(t,line,output);
        fclose(output);
        if(line[0] == '\n') remove(fileName);

        clock_gettime(CLOCK_REALTIME,&end);
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        switch (opt){
        case 1:
            yellow();
            printf("Elapsed time for command %d: %.6f s\n",commandNumber, elapsed);
            reset();
            break;
        case 2:
            char resultFile[FILENAMESIZE];
            snprintf(resultFile,FILENAMESIZE,"testResults/command%d_Info.txt",commandNumber);
            FILE* f = fopen(resultFile,"w");
            fprintf(f,"Command %d Info:\nExecution time: %.6f s\nQuery number: %d\n",commandNumber,elapsed,queryNumber);
            fclose(f);
            break;
        default:
            break;
        }
        
        commandNumber++;
    }

    fclose(queryFile);

}

void handle(Tables* t,char* line,FILE* output){

    int queryNumber =getQueryNumber(line);
    int format = getFormat(line,queryNumber);
    
    switch (queryNumber)
    {
    case 1:
        formatQuery1(t,line,format,output);
        break;
    
    case 2:
        formatQuery2(t,line,format,output);
        break;
    
    case 3:
        formatQuery3(t,line,format,output);
        break;
    
    case 4:
        formatQuery4(t,line,format,output);
        break;
    
    case 5:
        formatQuery5(t,line,format,output);
        break;
    
    case 6:
        formatQuery6(t,line,format,output);
        break;
    
    case 7:
        formatQuery7(t,line,format,output);
        break;
    
    case 8:
        formatQuery8(t,line,format,output);
        break;
    
    case 9:
        formatQuery9(t,line,format,output);
        break;
    
    case 10:
        formatQuery10(t,line,format,output);
        break;
    default:
        break;
    }
}
