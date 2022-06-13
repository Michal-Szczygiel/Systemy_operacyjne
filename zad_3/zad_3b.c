#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int sig;

    if (argc != 4) {
        printf("Blad argumentow. Podaj {numer sygnalu} {opcje obslugi - 1, 2, 3} {sciezka do programu}\nOpcje:\n\t1 - domyslnie\n\t2 - ignorowanie\n\t3 - przechwycenie\n");
        exit(EXIT_FAILURE);
    }

    sig = atoi(argv[1]);

    pid_t pid = getpid();
    printf("PID procesu macierzystego: %d\n", pid);

    switch (pid = fork()) {
        case -1:
            perror("Nie mozna utworzyc procesu potomnego!\n");
            _exit(EXIT_FAILURE);

        case 0:
            pid = getpid();
            printf("PID procesu potomnego: %d\n", pid);
            if (execlp(argv[3], argv[3], argv[1], argv[2], NULL) == -1) {
                perror("execlp error!\n");
            }
            _exit(2);

        default:
            sleep(1); // Czekam na zarejestrowanie metody obsługi sygnałów w wątku potomnym

            while (kill(pid, 0) == -1) {
                sleep(1); // sprawdzenie errno zamiast sleep
            }
            printf("Wyslano sygnal do procesu potomnego!\n");

            if (kill(pid, sig) == -1) {
                perror("Wyslanie sygnalu do grupy zakonczylo sie blendem!\n");
                _exit(EXIT_FAILURE);
            }

            if (wait(NULL) == -1) { // Oczekiwanie, głównie po to aby pokazać co się stanie w przypadku ignorowania sygnału przez proces potomny
                perror("Czekanie na proces potomny nie powiodlo sie!\n");
                _exit(EXIT_FAILURE);
            }
    }

    return 0;
}
