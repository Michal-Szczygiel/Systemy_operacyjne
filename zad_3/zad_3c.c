#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int sig;
    pid_t pgid;

    if (argc != 4) {
        printf("Blad argumentow. Podaj {numer sygnalu} {opcje obslugi - 1, 2, 3} {sciezka do programu}\nOpcje:\n\t1 - domyslnie\n\t2 - ignorowanie\n\t3 - przechwycenie\n");
        exit(EXIT_FAILURE);
    }

    sig = atoi(argv[1]); // sprawdzenie poprawności numeru sygnału odbywa się w procesach potomnych

    switch (pgid = fork()) {
        case -1:
            perror("Nie mozna utworzyc procesu potomnego!\n");
            _exit(EXIT_FAILURE);

        case 0:
            if (execlp("./program.x", "./program.x", argv[1], argv[2], argv[3], NULL) == -1) {
                perror("execlp error!\n");
            }
            _exit(2);

        default:
            sleep(1);

            if (kill(-pgid, 0) == -1) {
                if (errno == ESRCH) {
                    perror("Nie udalo sie utworzyc procesu potomnego!\n");
                    _exit(EXIT_FAILURE);
                }
            }

            if (kill(-pgid, sig) == -1) {
                perror("Wyslanie sygnalu do grupy zakonczylo sie blendem!\n");
                _exit(EXIT_FAILURE);
            }

            if (wait(NULL) == -1) {
                perror("Czekanie na proces potomny nie powiodlo sie!\n");
                _exit(EXIT_FAILURE);
            }
    }

    return 0;
}
