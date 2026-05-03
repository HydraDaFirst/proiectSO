Hydra
hydradamane
Online

Hydra — 4/21/2026 12:16 AM
-----------------
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

message.txt
3 KB
Hydra — 4/21/2026 11:10 PM
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

message.txt
3 KB
Hydra — 4/24/2026 10:10 PM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

message.txt
8 KB
Hydra — 4/25/2026 2:54 PM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

message.txt
10 KB
Hydra — 4/25/2026 5:58 PM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

message.txt
13 KB
Hydra — 4/26/2026 3:18 PM
wda
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

message.txt
16 KB
Hydra — Yesterday at 3:19 PM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

message.txt
18 KB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
volatile sig_atomic_t ruleaza = 1;
void handle_signal(int sig){
    if(sig == SIGUSR1){
        char *buf = "Un raport nou a fost adaugat!\n";
        write(STDOUT_FILENO, buf, strlen(buf));
    }
    else if(sig == SIGINT){
        char *buf = "Semnalul de oprire a fost primit. Monitorul se inchide.\n";
        write(STDOUT_FILENO, buf, strlen(buf));
        ruleaza = 0;
    }
}
int main(){
    pid_t pid = getpid();

    int f = open(".monitor_pid", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    char pid_string[20];
    sprintf(pid_string, "%d", pid);
    write(f, pid_string, strlen(pid_string));
    close(f);
    printf("Monitor pornit cu ID-ul %d\n", pid);

    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    while(ruleaza){
        pause();
    }
    if(unlink(".monitor_pid") == 0){
        printf("Monitor sters cu succes!\n");
    }
    else printf("Eroare la stergerea monitorului!\n");
    return 0;
}
﻿
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#define MAX_PATH 250
#define string_size 300
typedef struct report{
    int report_ID;
    char inspector_Name[50];
    float x, y;
    char issue_Category[50];
    int severity_Level;
    time_t timestamp;
    char description_Text[50];
}report;
void get_path(const char *district, const char *file, char *path){
    strcpy(path, "");
    strcat(path, district);
    strcat(path, "/");
    strcat(path, file);
}
void ensure_file(const char *path, mode_t permisiuni){
    int f = open(path, O_CREAT | O_RDWR, permisiuni);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    else{
        chmod(path, permisiuni);
        close(f);
    }
}
int has_permission(const char *path, const char *role, mode_t manager_bits, mode_t inspector_bits){
    struct stat st;
    if(stat(path, &st) == -1){
        perror(NULL);
        exit(-1);
    }
    if(strcmp(role, "manager") == 0)
        return (st.st_mode & manager_bits);
    if(strcmp(role, "inspector") == 0)
        return (st.st_mode & inspector_bits);
    return 0;
}
int has_permission_district(const char *district, const char *role, mode_t manager_bits, mode_t inspector_bits){
    struct stat st;
    if(stat(district, &st) == -1){
        perror(NULL);
        exit(-1);
    }
    if(strcmp(role, "manager") == 0)
        return (st.st_mode & manager_bits);
    if(strcmp(role, "inspector") == 0)
        return (st.st_mode & inspector_bits);
    return 0;
}
void add(const char *district, const char *user, const char *role){
    char path[MAX_PATH];
    get_path(district, "reports.dat", path);        //open reports.dat in read-write mode + append
    if(has_permission(path, role, S_IWUSR, S_IWGRP) == 0){
        printf("Eroare! Nu aveti permisiune pentru a apela aceasta comanda!\n");
        return;
    }
    int f = open(path, O_RDWR | O_APPEND);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    report new_rep;

    struct stat st;
    int current_id = 1;
    if (stat(path, &st) == 0) {
        current_id = (st.st_size / sizeof(report)) + 1;
    }
    new_rep.report_ID = current_id;

    printf("X : ");
    scanf("%f", &new_rep.x);

    printf("Y : ");
    scanf("%f", &new_rep.y);

    printf("Category (road/lighting/flooding/other): ");
    scanf("%s", new_rep.issue_Category);

    printf("Severity level (1/2/3) : ");
    scanf("%d", &new_rep.severity_Level);

    printf("Description : ");
    getchar();
    fgets(new_rep.description_Text, 50, stdin);
    new_rep.description_Text[strcspn(new_rep.description_Text, "\n")] = 0;

    strcpy(new_rep.inspector_Name, user);

    new_rep.timestamp = time(NULL);

    write(f, &new_rep, sizeof(report));
    close(f);
    printf("report adaugat cu succes\n");     //append la new_rep + inchidere file
}
void bits_to_string(mode_t bits, char *string){
    if((bits & S_IRUSR) !=0)
        string[0] = 'r';
    else string[0] = '-';
    if((bits & S_IWUSR) !=0)
        string[1] = 'w';
    else string[1] = '-';
    if((bits & S_IXUSR) !=0)
        string[2] = 'x';
    else string[2] = '-';

    if((bits & S_IRGRP) !=0)
        string[3] = 'r';
    else string[3] = '-';
    if((bits & S_IWGRP) !=0)
        string[4] = 'w';
    else string[4] = '-';
    if((bits & S_IXGRP) !=0)
        string[5] = 'x';
    else string[5] = '-';

    if((bits & S_IROTH) !=0)
        string[6] = 'r';
    else string[6] = '-';
    if((bits & S_IWOTH) !=0)
        string[7] = 'w';
    else string[7] = '-';
    if((bits & S_IXOTH) !=0)
        string[8] = 'x';
    else string[8] = '-';
    string[9] = '\0';
}
void list(const char *district, const char *role){
    char path[MAX_PATH];
    get_path(district, "reports.dat", path);
    if(has_permission(path, role, S_IRUSR, S_IRGRP) == 0){
        printf("Eroare! Nu aveti permisiune pentru a apela aceasta comanda!\n");
        return;
    }
    char string[string_size];
    struct stat st;
    if(stat(path, &st) == -1){
        perror(NULL);
        exit(-1);
    }
    bits_to_string(st.st_mode, string);
    printf("File: %s\n", path);
    printf("Size: %ld bytes\n", st.st_size);
    printf("Permissions: %s\n", string);
    printf("Last modified: %s\n", ctime(&st.st_mtime));

    int f = open(path, O_RDONLY);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    report r;
    while(read(f, &r, sizeof(report)) > 0){
        printf("Report ID: %d\n", r.report_ID);
        printf("Inspector name: %s\n", r.inspector_Name);
        printf("GPS coordinates: %f %f\n", r.x, r.y);
        printf("Issue category: %s\n", r.issue_Category);
        printf("Severity level: %d\n", r.severity_Level);
        char *mod_time = ctime(&r.timestamp);
        mod_time[strcspn(mod_time, "\n")] = 0;
        printf("Timestamp: %s\n", mod_time);
        printf("Description text: %s\n", r.description_Text);
        printf("\n");
    }
    close(f);
}
void view(const char *district, const char *role, int id){
    char path[MAX_PATH];
    get_path(district, "reports.dat", path);
    if(has_permission(path, role, S_IRUSR, S_IRGRP) == 0){
        printf("Eroare! Nu aveti permisiune pentru a apela aceasta comanda!\n");
        return;
    }
    char string[string_size];
    struct stat st;
    if(stat(path, &st) == -1){
        perror(NULL);
        exit(-1);
    }
    bits_to_string(st.st_mode, string);
    printf("File: %s\n", path);
    printf("Size: %ld bytes\n", st.st_size);
    printf("Permissions: %s\n", string);
    printf("Last modified: %s\n", ctime(&st.st_mtime));

    int f = open(path, O_RDONLY);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    report r;
    int gasit = 0;
    while(read(f, &r, sizeof(report)) > 0){
        if(r.report_ID == id){
            gasit = 1;
            break;
        }
    }
    if(gasit == 1){
        printf("Report ID: %d\n", r.report_ID);
        printf("inspector name: %s\n", r.inspector_Name);
        printf("GPS coordinates: %f %f\n", r.x, r.y);
        printf("Issue category: %s\n", r.issue_Category);
        printf("Severity level: %d\n", r.severity_Level);
        char *mod_time = ctime(&r.timestamp);
        mod_time[strcspn(mod_time, "\n")] = 0;
        printf("Timestamp: %s\n", mod_time);
        printf("Description text: %s\n", r.description_Text);
        printf("\n");
    }
    close(f);
}
void symlinks(const char *district){
    char path[MAX_PATH];
    char nume_link[string_size];
    get_path(district, "reports.dat", path);
    strcpy(nume_link, "");
    strcat(nume_link, "active_reports-");
    strcat(nume_link, district);

    struct stat lst, st;
    if(lstat(nume_link, &lst) == 0){
        if(S_ISLNK(lst.st_mode)){
            if(stat(nume_link, &st) == -1){
                printf("Warning! Ai identificat un dangling link :  %s\n", nume_link);
                if(unlink(nume_link) == 0)
                    printf("Dangling link ul a fost curatat cu succes\n");
                else printf("Eroare la curatarea dangling link ului\n");
            }
            else{
                printf("Symlink ul %s exista si e valid\n", nume_link);
            }
        }
    }else{
            if(symlink(path, nume_link) == 0)
                printf("Symlink creat : %s -> %s\n", nume_link, path);
            else printf("Eroare la creear symlink\n");
    }
}
void log_write(const char *district, const char *role, const char *user, const char *operation){
    char path[MAX_PATH];
    get_path(district, "logged_district", path);

    struct stat st;
    if(stat(path, &st) == -1)
        return;

    if(has_permission(path, role, S_IWUSR, 0) == 0){
        printf("Eroare! Doar managerul are voie sa scrie.\n");
        return;
    }
    int f = open(path, O_WRONLY | O_APPEND);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    char buf[string_size];
    strcpy(buf, "");
    time_t now_time = time(NULL);
    char *timp = ctime(&now_time);
    timp[strcspn(timp, "\n")] = 0;
    strcat(buf, timp);

    strcat(buf, " User: ");
    strcat(buf, user );

    strcat(buf, " Role: ");
    strcat(buf, role );

    strcat(buf, " Operation: ");
    strcat(buf, operation);
    strcat(buf, " ");
    write(f, buf, strlen(buf));
    close(f);
}
void remove_report(const char *district, const char *role, int id_cautat){
    char path[MAX_PATH];
    get_path(district, "reports.dat", path);

    struct stat st;
    if(stat(path, &st) == -1)
        return;

    if(has_permission(path, role, S_IWUSR, 0) == 0){
        printf("Eroare! Doar managerul poate sterge un report.\n");
        return;
    }
    int f = open(path, O_RDWR);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    int nr_total_reports = st.st_size / sizeof(report);

    report r;
    int index_real = -1;
    int i = 0;
    while(read(f, &r, sizeof(report)) > 0) {
        if(r.report_ID == id_cautat) {
            index_real = i;
            break;
        }
        i++;
    }

    if(index_real == -1) {
        printf("Raportul cu ID-ul %d nu a fost gasit.\n", id_cautat);
        close(f);
        return;
    }

    for(int i = index_real; i < nr_total_reports - 1; i++){
        lseek(f, (i + 1) * sizeof(report) , SEEK_SET);
        read(f, &r, sizeof(report));

        lseek(f, i * sizeof(report), SEEK_SET);
        write(f, &r, sizeof(report));
    }

    if(ftruncate(f, (nr_total_reports - 1) * sizeof(report)) == -1)
        perror("Eroare la truncate!\n");
    close(f);
    printf("Raport sters cu succes!\n");
}
void update_threshold(const char *district, const char *role, int new_severity_threshold){
    char path[MAX_PATH];
    get_path(district, "district.cfg", path);

    struct stat st;
    if(stat(path, &st) == -1){
        perror(NULL);
        exit(-1);
    }

    if(has_permission(path, role, S_IWUSR, 0) == 0){
        printf("Eroare! Doar managerul poate da update la threshold.\n");
        return;
    }
    mode_t permisiuni_actuale = st.st_mode & 0777;
    if(permisiuni_actuale != 0640){
        printf("Eroare! Permisiuni modificate!\n");
        return;
    }

    int f = open(path, O_WRONLY);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    char buf[20];
    sprintf(buf, "%d", new_severity_threshold);
    write(f, buf, strlen(buf));
    close(f);
    printf("Treshold updatat cu succes!\n");
}
int parse_condition(const char *input, char *field, char *op, char *value) {
    if (sscanf(input, "%[^:]:%[^:]:%s", field, op, value) == 3) {
        return 1;
    }
    return 0;
}
int match_condition(report *r, const char *field, const char *op, const char *value) {
    double report_val = 0;
    double target_val = 0;
    int is_string_comp = 0;

    if (strcmp(field, "severity") == 0) {
        report_val = r->severity_Level;
        target_val = atoi(value);
    } else if (strcmp(field, "timestamp") == 0) {
        report_val = (double)r->timestamp;
        target_val = atol(value);
    } else if (strcmp(field, "category") == 0) {
        is_string_comp = 1;
    } else if (strcmp(field, "inspector") == 0) {
        is_string_comp = 1;
    } else {
        return 0; // Camp necunoscut
    }

    if (is_string_comp) {
        const char *actual_str = (strcmp(field, "category") == 0) ? r->issue_Category : r->inspector_Name;
        if (strcmp(op, "==") == 0) return strcmp(actual_str, value) == 0;
        if (strcmp(op, "!=") == 0) return strcmp(actual_str, value) != 0;
        return 0;
    } else {
        if (strcmp(op, "==") == 0) return report_val == target_val;
        if (strcmp(op, "!=") == 0) return report_val != target_val;
        if (strcmp(op, "<") == 0)  return report_val < target_val;
        if (strcmp(op, "<=") == 0) return report_val <= target_val;
        if (strcmp(op, ">") == 0)  return report_val > target_val;
        if (strcmp(op, ">=") == 0) return report_val >= target_val;
    }
    return 0;
}
void filter_reports(const char *district, const char *role, int argc, char *argv[], int start_index) {
    char path[MAX_PATH];
    get_path(district, "reports.dat", path);

    if(has_permission(path, role, S_IRUSR, S_IRGRP) == 0) {
        printf("Eroare! Lipsa permisiuni citire.\n");
        return;
    }

    int f = open(path, O_RDONLY);
    if(f < 0) { perror(NULL); return; }

    report r;
    while(read(f, &r, sizeof(report)) > 0) {
        int all_match = 1;
        for(int i = start_index; i < argc; i++) {
            char field[50], op[10], value[100];
            if(parse_condition(argv[i], field, op, value)) {
                if(!match_condition(&r, field, op, value)) {
                    all_match = 0;
                    break;
                }
            }
        }
        if(all_match) {
            printf("[%d] %s | %s | Sev: %d\n", r.report_ID, r.issue_Category, r.inspector_Name, r.severity_Level);
        }
    }
    close(f);
}
void remove_district(const char *district, const char *role){
    if(has_permission_district(district, role, S_IXUSR, 0) == 0){
        printf("Eroare! Doar managerul poate sterge un district.\n");
        return;
    }
    char symlink_name[250];
    strcpy(symlink_name, "");
    strcat(symlink_name, "active_reports-");
    strcat(symlink_name, district);
    char s = fork();
    if(s == 0){
        execlp("rm", "rm", "-rf", district, NULL);
    }
    if(s != 0){
        waitpid(s, NULL, 0);
        unlink(symlink_name);
    }
    printf("District sters cu succes!\n");
}
int main(int argc, char *argv[]){
    char *role = NULL;
    char *user = NULL;
    char *command = NULL;
    char *district = NULL;
    char *id_functions = NULL;
    for(int i = 1 ; i < argc; i++){
        if(strcmp(argv[i], "--role") == 0)
            role = argv[++i];
        else if(strcmp(argv[i], "--user") == 0)
            user = argv[++i];
        else if(command == NULL)
            command = argv[i];
        else if(district == NULL)
            district = argv[i];
        else if(id_functions == NULL)
            id_functions = argv[i];
    }
    if(strcmp(command, "--add") == 0){
        if(mkdir(district, 0750) == -1 && errno != EEXIST){
            printf("Eroare creare director\n");
            return 1;
        }
        chmod(district, 0750);
        char path[MAX_PATH];
        get_path(district, "reports.dat", path);
        ensure_file(path, 0664);
        get_path(district, "district.cfg", path);
        ensure_file(path, 0640);
        get_path(district, "logged_district", path);
        ensure_file(path, 0644);

        add(district, user, role);
        symlinks(district);
        log_write(district, role, user, "add\n");

        int f = open(".monitor_pid", O_RDONLY);
        int sig_sent_succ = 0;
        if(f < 0){
            log_write(district, role, user, "monitorul nu a putut fi apelat.\n");
        }
        else{
            char buf[20];
            int bytes = read(f, buf, sizeof(buf) - 1);
            if(bytes > 0){
                buf[bytes] = '\0';
                pid_t pid = atoi(buf);

                if(kill(pid, SIGUSR1) == 0)
                    sig_sent_succ = 1;
            }
        }
        close(f);
        if(sig_sent_succ == 1)
            log_write(district, role, user, "monitor notificat cu succes!\n");
        else log_write(district, role, user, "monitorul nu a putut fi notificat.\n");
    }
    else if(strcmp(command, "--list") == 0){
        list(district, role);
        symlinks(district);
        log_write(district, role, user, "list");
    }
    else if(strcmp(command, "--view") == 0){
        if(id_functions != NULL){
            int id = atoi(id_functions);
            view(district, role, id);
            symlinks(district);
            log_write(district, role, user, "view");
        }
        else printf("Eroare! Nu ati specificat un id.\n");
    }
    else if(strcmp(command, "--remove_report") == 0){
        if(id_functions != NULL){
            int id = atoi(id_functions);
            remove_report(district, role, id);
            symlinks(district);
            log_write(district, role, user, "remove_report");
        }
    }
    else if(strcmp(command, "--update_threshold") == 0){
        if(id_functions != NULL){
            int new_threshold = atoi(id_functions);
            update_threshold(district, role, new_threshold);
            symlinks(district);
            log_write(district, role, user, "update threshold");
        }
    }
    else if(strcmp(command, "--filter") == 0) {
        int prima_conditie = 0;
        for(int j = 1; j < argc; j++) {
            if(strcmp(argv[j], district) == 0) {
                prima_conditie = j + 1;
                break;
            }
        }
        if(prima_conditie < argc) {
            filter_reports(district, role, argc, argv, prima_conditie);
            log_write(district, role, user, "filter");
        } else {
            printf("Eroare! Nu ați specificat nicio condiție pentru filtru.\n");
        }
    }
    else if(strcmp(command, "--remove_district") == 0){
        remove_district(district, role);
    }
    return 0;
}