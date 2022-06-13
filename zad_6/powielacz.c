#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#include "sem_lib.h"

#define PROCESS_NUMBER_BUFFER 32

char *SEMAPHORE_NAME;

void signal_handler(int sig) {
    exit(EXIT_FAILURE);
}

void remove_semaphore() {
    if (sem_unlink(SEMAPHORE_NAME) == -1) {
        perror("[PROCES MACIERZYSTY] Nie mozna usunac semafora!\n");
    } else {
        printf("[PROCES MACIERZYSTY] Usunieto semafor!\n");
    }

    printf("Zakonczono!\n");
}

int main(int argc, char *argv[]) {
    // {nazwa programu} {liczba procesów} {liczba sekcji krytycznych} {nazwa semafora}
    if (argc != 5) {
        printf("[PROCES MACIERZYSTY] Podaj poprawne parametry: {nazwa programu} {liczba procesow} {liczba sekcji krytycznych} {nazwa semafora}\n");
        exit(EXIT_FAILURE);
    } else {
        printf("[PROCES MACIERZYSTY] Uruchamiam z nastepujacymi parametrami:\n\
\t- nazwa programu: %s\n\t- liczba procesow: %s\n\t- liczba sekcji krytycznych: %s\n\t- nazwa semafora: %s\n", argv[1], argv[2], argv[3], argv[4]);
    }

    SEMAPHORE_NAME = argv[4];
    int processes_number = atoi(argv[2]);
    int sections = atoi(argv[3]);
    char process_number_buffer[PROCESS_NUMBER_BUFFER];

    // Utworzenie semafora
    sem_t *semaphore = open_semaphore_parent(argv[4], O_CREAT | O_EXCL, 0664, 1);

    // Zarejestrowanie metody atexit
    if (atexit(remove_semaphore) != 0) {
        perror("[PROCES MACIERZYSTY] Nie mozna zarejestrowac metody atexit!\n");
        exit(EXIT_FAILURE);
    }

    // Zarejestrowanie signal handlingu
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("[PROCES MACIERZYSTY] Nie mozna zarejestrowac przechwytywania sygnalu SIGINT!\n");
        exit(EXIT_FAILURE);
    }

    // Utworzenie pliku number.txt
    initialize_number_file_parent("number.txt");

    // Uruchomienie procesów potomnych
    for (int i = 0; i < processes_number; i++) {
        pid_t process_id = fork();

        if (process_id == 0) {
            // Proces potomny
            sprintf(process_number_buffer, "%d", i);
            if (execlp(argv[1], argv[1], process_number_buffer, argv[3], argv[4], NULL) == -1) {
                perror("[PROCES POTOMNY] Nie mozna uruchomic programu!\n");
                _exit(EXIT_FAILURE);
            }
        } else if (process_id == -1) {
            perror("[PROCES MACIERZYSTY] Nie mozna utworzyc procesu potomnego!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Oczekiwanie na procesy potomne
    for (int i = 0; i < processes_number; i++) {
        if (wait(NULL) == -1) {
            perror("[PROCES MACIERZYSTY] Czekanie na proces potomny nie powiodlo sie!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Zamknięcie semafora
    close_semaphore_parent(semaphore);

    // Sprawdzenie liczby z pliku
    compare_number_from_file("number.txt", processes_number, sections);

    return 0;
}
