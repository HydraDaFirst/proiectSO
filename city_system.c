#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
typedef struct report{
    int report_ID;
    char inspector_Name[50];
    float x, y;
    char issue_Category[50];
    int severity_Level;
    time_t timestamp;
    char description_Text[50];
}report;
int main(int argc, char *argv[]){
    if(strcmp(argv[5], "--add") == 0){
        int new_dir = mkdir(argv[6], 0750);
        if(new_dir != 0)
            printf("mnp\n");
        else printf("dwadaw\n");
    }
    report new_rep;
    printf("X : ");
    scanf("%f", &new_rep.x);
    printf("Y : ");
    scanf("%f", &new_rep.y);
    printf("Category (road/lighting/flooding/other): ");
    scanf("%s", new_rep.issue_Category);
    printf("Severity level (1/2/3) : ");
    scanf("%d", &new_rep.severity_Level);
    printf("Description : ");
    scanf("%s", new_rep.description_Text);
    return 0;
}
