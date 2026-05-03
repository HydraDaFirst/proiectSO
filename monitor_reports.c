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