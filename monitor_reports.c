#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
char err[] = "ERR";
char inf[] = "INF";
volatile sig_atomic_t ruleaza = 1;
void handle_signal(int sig){
    char buf[256];
    if(sig == SIGUSR1){
        strcpy(buf, inf);
        strcat(buf, " Un raport nou a fost adaugat!\n");
        write(STDOUT_FILENO, buf, strlen(buf));
    }
    else if(sig == SIGINT){
        strcpy(buf, inf);
        strcat(buf, " Semnalul de oprire a fost primit. Monitorul se inchide.\n");
        write(STDOUT_FILENO, buf, strlen(buf));
        ruleaza = 0;
    }
}
int main(){
    pid_t pid = getpid();

    int fd = open(".monitor_pid", O_RDONLY, 0644);
    if(fd >= 0){
        char buf[20];
        int bytes = read(fd, buf, sizeof(buf) - 1);
        if(bytes > 0){
            buf[bytes] = '\0';
            pid_t pid_existent = atoi(buf);
        printf("%s Monitorul cu pid ul %d deja exista\n", err, pid_existent);
        fflush(stdout);
        close(fd);
        return 1;
        }
    }
    int f = open(".monitor_pid", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(f < 0){
        perror(NULL);
        exit(-1);
    }
    char pid_string[20];
    sprintf(pid_string, "%d", pid);
    write(f, pid_string, strlen(pid_string));
    close(f);
    printf("%s Monitor pornit cu ID-ul %d\n", inf, pid);
    fflush(stdout);

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
        printf("%s Monitor sters cu succes!\n", inf);
    }
    else printf("%s Eroare la stergerea monitorului!\n", err);
    return 0;
}
