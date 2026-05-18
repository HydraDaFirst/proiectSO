#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
typedef struct report{
    int report_ID;
    char inspector_Name[50];
    float x, y;
    char issue_Category[50];
    int severity_Level;
    time_t timestamp;
    char description_Text[50];
}report;
typedef struct inspector_score{
    char nume[100];
    int scor;
}inspector_score;
int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Eroare! Numar invalid de argumente.\n");
        exit(-1);
    }

    char *district = argv[1];
    char path[256];
    sprintf(path, "%s/reports.dat", district);
    int f =  open(path, O_RDONLY);
    if(f < 0){
        return 0;
    }

    report r;
    inspector_score inspectori[100];
    int nr_inspectori = 0;
    while(read(f, &r, sizeof(report)) > 0){
        int gasit = 0;
        for(int i = 0; i < nr_inspectori; i++){
            if(strcmp(inspectori[i].nume, r.inspector_Name) == 0){
                inspectori[i].scor += r.severity_Level;
                gasit = 1;
                break;
            }
        }
        if(gasit == 0 && nr_inspectori < 100){
            strcpy(inspectori[nr_inspectori].nume, r.inspector_Name);
            inspectori[nr_inspectori].scor = r.severity_Level;
            nr_inspectori++;
        }
    }
    close(f);
    for(int i = 0; i < nr_inspectori; i++){
        printf("Inspector: %s, workload score: %d\n", inspectori[i].nume, inspectori[i].scor);
    }
    fflush(stdout);
    return 0;
}
