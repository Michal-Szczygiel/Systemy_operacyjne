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
        if (write(STDOUT_FILENO, "[KONSUMENT] Blad argumentow!\nPodaj {nazwe potoku} {nazwe tworzonego pliku}\n", 76) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    // Utworzenie pliku wynikowego
    int output_file = open(argv[2], O_WRONLY | O_CREAT, 0777);

    // Otwarcie kolejki
    int fifo = open(argv[1], O_RDONLY);

    if (output_file == -1) {
        perror("[KONSUMENT] Nie mozna utworzyc pliku na dane!\n");
        exit(EXIT_FAILURE);
    }

    if (fifo == -1) {
        perror("[KONSUMENT] Nie mozna otworzyc potoku!\n");
        exit(EXIT_FAILURE);
    }

    // Czytanie z kolejki
    int received_bytes = read(fifo, buffer, BUFFER_SIZE);

    srand(time(0) + 13467);

    while (received_bytes != -1) {
        if (received_bytes == 0) {
            if (write(STDOUT_FILENO, "[KONSUMENT] Dane sie skonczyly!\n", 33) == -1) {
                perror("Blad wypisywania komunikatu!\n");
                exit(EXIT_FAILURE);
            }

            break;
        }

        sleep(1 + rand() % 3);
        buffer[received_bytes] = '\0';
        sprintf(print_buffer, "[KONSUMENT] Odebrano: %d bajtow. Tresc: {%s}\n", received_bytes, buffer);

        // Pisanie do pliku wynikowego
        if (write(output_file, buffer, received_bytes) == -1) {
            perror("[KONSUMENT] Blad podczas zapisu do pliku!\n");
            exit(EXIT_FAILURE);
        }

        // Wypisanie komunikatu o odczytanej treści
        if (write(STDOUT_FILENO, print_buffer, strlen(print_buffer)) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }

        // Czytanie z kolejki
        received_bytes = read(fifo, buffer, BUFFER_SIZE);
    }

    // Zamknięcie pliku wynikowego
    if (close(output_file) == -1) {
        perror("[KONSUMENT] Blad podczas zamykania pliku!\n");
        exit(EXIT_FAILURE);
    } else {
        if (write(STDOUT_FILENO, "[KONSUMENT] Zamknieto plik na dane.\n", 37) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Zamknięcie kolejki
    if (close(fifo) == -1) {
        perror("[KONSUMENT] Blad podczas zamykania potoku!\n");
        exit(EXIT_FAILURE);
    } else {
        if (write(STDOUT_FILENO, "[KONSUMENT] Zamknieto potok.\n", 30) == -1) {
            perror("Blad wypisywania komunikatu!\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
