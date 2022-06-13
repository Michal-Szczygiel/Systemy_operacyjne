#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

extern const char * const sys_siglist[];

void signal_handler(int sig) {
    char buffer[128];
    sprintf(buffer, "Przechwycono syngal o numerze: %d (%s)", sig, sys_siglist[sig]);
    printf("%s\n", buffer);
}

int main(int argc, char *argv[]) {
    int sig, opt;
    char buffer[128];

    if (argc != 3) {
        printf("Blad argumentow. Podaj {numer sygnalu} {opcje obslugi - 1, 2, 3}\nOpcje:\n\t1 - domyslnie\n\t2 - ignorowanie\n\t3 - przechwycenie\n");
        exit(EXIT_FAILURE);
    }

    sig = atoi(argv[1]);
    opt = atoi(argv[2]);

    if (sig < 0 || sig > 63) {
        printf("Podano niepoprawny numer sygnalu!\n");
        exit(EXIT_FAILURE);
    }

    printf("PID procesu: %d\nObsluga sygnalu: sygnal - %s, opcja - %d\n", getpid(), sys_siglist[sig], opt);

    switch (opt) {
        case 1:
            if (signal(sig, SIG_DFL) == SIG_ERR) {
                sprintf(buffer, "Funkcja signal ma problem z %s!\n", sys_siglist[sig]);
                perror(buffer);
                exit(EXIT_FAILURE);
            }
            break;

        case 2:
            if (signal(sig, SIG_IGN) == SIG_ERR) {
                sprintf(buffer, "Funkcja signal ma problem z %s!\n", sys_siglist[sig]);
                perror(buffer);
                exit(EXIT_FAILURE);
            }
            break;

        case 3:
            if (signal(sig, signal_handler) == SIG_ERR) {
                sprintf(buffer, "Funkcja signal ma problem z %s!\n", sys_siglist[sig]);
                perror(buffer);
                exit(EXIT_FAILURE);
            }
            break;

        default:
            printf("Bledna opcja obslugi sygnalu!\n");
    }

    if (pause() == -1) {
        perror("Errno: EINTR!\n");
    }

    sleep(2);

    return 0;
}
