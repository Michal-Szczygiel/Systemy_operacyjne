#define _POSIX_C_SOURCE 200112L
#define PROC_NUMB 3

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

extern const char * const sys_siglist[];

int main(int argc, char *argv[]) {
    char buffer[128];
    int sig = atoi(argv[1]); // sprawdzenie poprawności numeru sygnału odbywa się w procesach potomnych
    int proc_stat;

    if (setpgid(0, 0) != 0) {
        perror("Nie udalo sie ustanowic nowego lidera grupy!\n");
        _exit(EXIT_FAILURE);
    }

    if (signal(sig, SIG_IGN) == SIG_ERR) {
        sprintf(buffer, "Funkcja signal ma problem z %s!\n", sys_siglist[sig]);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }

    for (int i = 0; i < PROC_NUMB; i++) {
        switch (fork()) {
            case -1:
                perror("Nie mozna utworzyc procesu potomnego!\n");
                _exit(EXIT_FAILURE);
            case 0:
                if (execlp(argv[3], argv[3], argv[1], argv[2], NULL) == -1) {
                    perror("execlp error!\n");
                }
                _exit(2);

            default:
                for (int i = 0; i < PROC_NUMB; i++) {
                    if (wait(&proc_stat) == -1) {
                        perror("Czekanie na proces potomny nie powiodlo sie!\n");
                        _exit(EXIT_FAILURE);    // Może się zdażyć sytuacja, że procesy potomne umrą wcześniej, 
                                                // wtedy następuje zakończenie procesu macierzystego
                    }

                    printf("Proces potomny zakonczyl się ze statusem: %d\n", proc_stat);
                }
                _exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
