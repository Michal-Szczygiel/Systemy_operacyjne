#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 128
#define PRINT_BUFFER 256

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE + 1], print_buffer[PRINT_BUFFER];

    if (argc != 3) {
        if (write(STDOUT_FILENO, "[PRODUCENT] Blad argumentow!\nPodaj {nazwe potoku} {nazwe pliku z danymi}\n", 74) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    // Otwarcie pliku z danymi do czytania
    int data_file = open(argv[2], O_RDONLY);

    // Otwarcie fifo do pisania
    int fifo = open(argv[1], O_WRONLY);

    if (data_file == -1) {
        perror("[PRODUCENT] Nie mozna otworzyc pliku z danymi!\n");
        exit(EXIT_FAILURE);
    }

    if (fifo == -1) {
        perror("[PRODUCENT] Nie mozna otworzyc potoku!\n");
        exit(EXIT_FAILURE);
    }

    // Czytanie z pliku
    int sended_bytes = read(data_file, buffer, BUFFER_SIZE);

    srand(time(0) + 97232);

    while (sended_bytes != 0) {
        if (sended_bytes == 0) {
            if (write(STDOUT_FILENO, "[PRODUCENT] Dane sie skonczyly!\n", 33) == -1) {
                perror("Blad wypisywania komunikatu!\n");
                exit(EXIT_FAILURE);
            }

            break;
        }

        sleep(1 + rand() % 3);
        buffer[sended_bytes] = '\0';
        sprintf(print_buffer, "[PRODUCENT] Przeslano: %d bajtow. Tresc: {%s}\n", sended_bytes, buffer);

        // Pisanie do fifo
        if (write(fifo, buffer, sended_bytes) == -1) {
            perror("[PRODUCENT] Blad podczas zapisu do potoku!\n");
            exit(EXIT_FAILURE);
        }

        // Wypisanie komunikatu o wysłanej treści
        if (write(STDOUT_FILENO, print_buffer, strlen(print_buffer)) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }

        // Czytanie z pliku
        sended_bytes = read(data_file, buffer, BUFFER_SIZE);
    }

    // Zamknięcie pliku z danymi
    if (close(data_file) == -1) {
        perror("[PRODUCENT] Blad podczas zamykania pliku!\n");
        exit(EXIT_FAILURE);
    } else {
        if (write(STDOUT_FILENO, "[PRODUCENT] Zamknieto plik z danymi.\n", 38) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Zamknięcie kolejki
    if (close(fifo) == -1) {
        perror("[PRODUCENT] Blad podczas zamykania potoku!\n");
        exit(EXIT_FAILURE);
    } else {
        if (write(STDOUT_FILENO, "[PRODUCENT] Zamknieto potok.\n", 30) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
