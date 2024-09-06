#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <time.h>
#include <sys/resource.h>
#include "../include/table.h"
#include "../include/handlequery.h"
#include "../include/util.h"

#define LINESIZE 1024
#define FILENAMESIZE 128

/* Função que obtem o número de queries que serão executadas no modo batch */
int numberOfCommands(char* filePath){
    FILE* f = fopen(filePath,"r");
    char* line = NULL;
    size_t len = 0;
    int count = 0;
    while(getline(&line,&len,f) != -1){
        count++;
    }
    if (line) free(line);
    fclose(f);
    return count;
}

/* Função que compara linha a linha os resultados das queries */
void compareFiles(char* expectedResultsPath,int max,int option){
    char expected[FILENAMESIZE];
    char result[FILENAMESIZE];
    for(int i=1;i<=max;i++){
        int lineNumber = 1;
        
        snprintf(expected,FILENAMESIZE,"%s/command%d_output.txt",expectedResultsPath,i);
        snprintf(result,FILENAMESIZE,"Resultados/command%d_output.txt",i);
        
        FILE* expectedFile = fopen(expected,"r");
        FILE* resultFile = fopen(result,"r");

        int keepReading = 1;
        int hasMistake = 0;

        char expectedLine[LINESIZE];
        char resultLine[LINESIZE];
        
        while(keepReading){
            if (fgets(expectedLine,LINESIZE,expectedFile) == NULL || fgets(resultLine,LINESIZE,resultFile) == NULL) keepReading = 0;
            else if (strcmp(expectedLine,resultLine) != 0){
                keepReading = 0;
                hasMistake = 1;
                if(option == 1){
                    red();
                    printf("Command %d error on line %d\n",i,lineNumber);
                    reset();
                }else{
                    char testResultFile[FILENAMESIZE];
                    snprintf(testResultFile,FILENAMESIZE,"testResults/command%d_Info.txt",i);
                    FILE* f = fopen(testResultFile,"a");
                    fprintf(f,"Error on line %d\n\n",lineNumber);
                }
            }
            lineNumber++;
        }
        if (!hasMistake){
            if(option == 1){
                green();
                printf("Command %d correct\n",i);
                reset();
            }else{
                char testResultFile[FILENAMESIZE];
                snprintf(testResultFile,FILENAMESIZE,"testResults/command%d_Info.txt",i);
                FILE* f = fopen(testResultFile,"a");
                fprintf(f,"Correct\n\n");
                fclose(f);
            }
        }

        fclose(expectedFile);
        fclose(resultFile);
    }
}

/* Função para escolher a formatação da resposta das execuções */
int chosenOption(char* option1,char* option2){
    char* choice = NULL;
    size_t len = 0;
    int keepReading = 1;
    char* choices[] = {option1,option2};
    int index = 0;
    while (keepReading){
        printf("\033c");
        printf("How do you wish to see the results?\n");
        printf(choices[index]);
        printf("Press enter to see other options or write anything to choose\n");
        getline(&choice,&len,stdin);
        if(strcmp("\n",choice) != 0) keepReading = 0;
        else index = index == 0 ? 1 : 0;
    }

    printf("\033c");

    if(choice) free(choice);
    return index + 1;
}

void printTestResults(int commands){
    printf("\n");
    for(int i=1;i<=commands;i++){
        char fileName[FILENAMESIZE];
        snprintf(fileName,FILENAMESIZE,"testResults/command%d_Info.txt",i);
        FILE* f = fopen(fileName,"r");
        char line[LINESIZE];
        while(fgets(line,LINESIZE,f)!=NULL){
            printf(line);
        }
        fclose(f);
    }
}

int main(int argc, char** argv){

    if (argc != 4){
        printf("Invalid number of arguments\n");
        return 1;
    }

    // opções para formatar o resultado da execução
    char* option1 = "Command 1 time: 0.3 s\n.\n.\n.\nCommand 100 time: 0.2 s\nCommand 1 correct\n.\n.\n.\nCommand 100 correct\n";
    char* option2 = "Command 1:\nTime: 0.3\nCorrect\n.\n.\n.\nCommand 100:\nTime: 0.2\nCorrect\n";

    int option = chosenOption(option1,option2);

    struct rusage r_usage;

    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME,&start);

    char* datasetPath = argv[1];

    Tables* tables = atributeTables(datasetPath);

    char* batchFile = argv[2];

    interpreter(tables,batchFile,option);

    char* expectedResultsPath = argv[3];

    int commands = numberOfCommands(batchFile);

    if (tables) destroyTables(tables);

    clock_gettime(CLOCK_REALTIME,&end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    
    compareFiles(expectedResultsPath,commands,option);
    
    if(option == 2){
        printTestResults(commands);
    }

    
    yellow();
    printf("Elapsed time: %.6f s\n", elapsed);

    getrusage(RUSAGE_SELF,&r_usage);
    printf("Memory used : %ld KB\n",r_usage.ru_maxrss / 1024);
    reset();
    return 0;
}
