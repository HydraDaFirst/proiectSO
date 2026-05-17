#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void start_monitor(){
    pid_t hub_mon = fork();
    if(hub_mon < 0){
        printf("Fork esuat.\n");
        return;
    }
    if(hub_mon == 0){
        int fd[2];
        pipe(fd);
        pid_t monitor_pid = fork();
        if(monitor_pid == 0){
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execlp("./monitor_reports", "./monitor_reports", NULL);
            perror("Eroare la execlp.\n");
            exit(-1);
        }
        else{
            close(fd[1]);
            char buf[128];
            while(1){
                int n = read(fd[0], buf, sizeof(buf) - 1);
                if(n > 0){
                    buf[n] = '\0';
                    printf("HUB MSG : %s", buf);
                    fflush(stdout);
                }
                else if(n == 0){
                    printf("Hub MSG : Monitor oprit, functia read retunreaza 0.\n");
                    break;
                }
                else {
                    printf("Hub MSG : Eroare la citirea din pipe.\n");
                    break;
                }
            }
            exit(0);
        }
    }
}
int main(void){
    char command[100];
    while(1){
        if(fgets(command, 100, stdin) == NULL)
            break;
        command[strcspn(command, "\n")] = 0;
        if(strcmp(command, "start_monitor") == 0)
            start_monitor();
        else if(strcmp(command, "exit") == 0)
            break;
        else printf("Comanda necunoscuta\n");
    }
    return 0;
}
