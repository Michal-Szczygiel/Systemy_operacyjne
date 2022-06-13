#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#include "sem_lib.h"

int main(int argc, char *argv[]) {
    // {numer procesu} {liczba sekcji} {nazwa semafora}
    if (argc != 4) {
        printf("[PROCES POTOMNY] Podaj odpowiednie parametry!\n");
        _exit(EXIT_FAILURE);
    }

    int process_number = atoi(argv[1]);
    int sections = atoi(argv[2]);

    // Otwarcie semafora
    sem_t *semaphore = open_semaphore_child(argv[3], O_CREAT, 0664, 1, process_number);

    srand(time(0) + process_number * 123);

    for (int i = 0; i < sections; i++) {
        // Odczekanie przed sekcją krytyczną
        sleep(rand() % 3 + 1);

        // Początek sekcji krytycznej
        wait_semaphore_child(semaphore, process_number);

        // Działanie w sekcji krytycznej
        printf("[PROCES POTOMNY %d] Wejscie do sekcji krytycznej.\n", process_number);

        // Inkrementacja liczby z pliku 'number.txt'
        increment_number_from_file_child("number.txt", process_number);

        printf("[PROCES POTOMNY %d] Wyjscie z sekcji krytycznej.\n", process_number);

        // Koniec sekcji krytycznej
        post_semaphore_child(semaphore, process_number);
        
        // Oczekanie po sekcji krytycznej
        sleep(rand() % 3 + 1);
    }

    // Zamknięcie semafora
    close_semaphore_child(semaphore, process_number);

    return 0;
}
