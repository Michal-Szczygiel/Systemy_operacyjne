#define _POSIX_C_SOURCE 200112L
#define PROC_NUMBER 3

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

extern const char * const sys_siglist[];

int main(int argc, char *argv[]) {
    pid_t pgid;
    int process_stat, sig;
    char buffer[128];

    if (argc != 4) {
        printf("Blad argumentow. Podaj {numer sygnalu} {opcje obslugi - 1, 2, 3} {sciezka do programu}\nOpcje:\n\t1 - domyslnie\n\t2 - ignorowanie\n\t3 - przechwycenie\n");
        exit(EXIT_FAILURE);
    }

    sig = atoi(argv[1]);

    switch (pgid = fork()) {
        case -1:
            perror("Nie mozna utworzyc procesu potomnego!\n");
            _exit(EXIT_FAILURE);

        case 0:
            if (setpgid(0, 0) != 0) {
                perror("Nie udalo sie ustanowic nowego lidera grupy!\n");
                _exit(EXIT_FAILURE);
            }

            if (signal(sig, SIG_IGN) == SIG_ERR) {
                sprintf(buffer, "Funkcja signal ma problem z %s!\n", sys_siglist[sig]);
                perror(buffer);
                _exit(EXIT_FAILURE);
            }

            for (int i = 0; i < PROC_NUMBER; i++) {
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
                        if (wait(&process_stat) == -1) {
                            perror("Czekanie na proces potomny nie powiodlo sie!\n");
                            _exit(EXIT_FAILURE);
                        }

                        printf("Status zakonczenia: %d\n------------------------------------\n", process_stat);
                }
            }

            _exit(EXIT_SUCCESS);

        default:
            for (int i = 0; i < PROC_NUMBER; i++) {
                sleep(1);
                while (kill(-pgid, 0) == -1) {
                    sleep(1);
                }

                kill(-pgid, SIGQUIT);
            }
    }

    return 0;
}
