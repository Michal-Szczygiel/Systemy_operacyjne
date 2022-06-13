#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

static char *fifo_name = NULL;

// Funkcja atexit zamykająca fifo
void close_fifo() {
    if (unlink(fifo_name) == -1) {
        perror("Nie mozna zniszczyc potoku!\n");
    } else {
        printf("Potok zostal zniszczony!\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Blad argumentow!\nPodaj {nazwe potoku} {nazwe programu producenta} {nazwe programu konsumenta} {nazwe pliku z danymi} {nazwe tworzonego pliku}\n");
        exit(EXIT_FAILURE);
    }

    fifo_name = argv[1];

    // Tworzenie fifo
    if (mkfifo(argv[1], 0644) == -1) {
        perror("Nie mozna utworzyc potoku!\n");
        exit(EXIT_FAILURE);
    }

    // Rejestrowanie metody atexit zamykającej fifo
    if (atexit(close_fifo) == -1) {
        perror("Nie mozna zarejestrowac procedury konczacej!\n");
        exit(EXIT_FAILURE);
    }

    // Tworzenie procesu potomnego
    switch (fork()) {
        case -1:
            perror("Nie mozna utworzyc procesu potomnego!\n");
            exit(EXIT_FAILURE);
        
        case 0:
            if (execlp(argv[2], argv[2], argv[1], argv[4], NULL) == -1) {
                perror("execlp error!\n");
            } // Proces producenta
            exit(EXIT_FAILURE);

        default:
            switch (fork()) {
                case -1:
                    perror("Nie mozna utworzyc procesu potomnego!\n");
                    exit(EXIT_FAILURE);

                case 0:
                    if (execlp(argv[3], argv[3], argv[1], argv[5], NULL) == -1) {
                        perror("execlp error!\n");
                    } // Proces konsumenta
            
                default:
                    // Oczekiwanie na zakończenie procesu potomnego (konsumenta)
                    if (wait(NULL) == -1) {
                        perror("Czekanie na proces potomny nie powiodlo sie!\n");
                        exit(EXIT_FAILURE);
                    }
            }

            // Oczekiwanie na zakończenie działania procesu potomnego (producenta)
            if (wait(NULL) == -1) {
                perror("Czekanie na proces potomny nie powiodlo sie!\n");
                exit(EXIT_FAILURE);
            }
    }

    return 0;
}
