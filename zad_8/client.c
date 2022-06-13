#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <time.h>

#include "utility.h"

static char CLIENT_QUEUE_NAME[128];

// Funkcja obsługi sygnału SIGINT
void sigint_handler(int signal) {
    exit(EXIT_FAILURE);
}

// Procedura atexit
void remove_queue() {
    printf("\n[KLIENT] Kończę działanie programu klienta!\n");

    if (mq_unlink(CLIENT_QUEUE_NAME) == -1) {
        perror("[KLIENT] Nie można usunąć kolejki z systemu!\n");
    } else {
        printf("[KLIENT] Kolejka została usunięta!\n");
    }
}

int main() {
    // Utworzenie kolejki klienta
    struct mq_attr client_queue_atr = {
        .mq_flags = 0,
        .mq_maxmsg = 8,
        .mq_msgsize = MESSAGE_SIZE,
        .mq_curmsgs = 0
    };

    // Utworzenie nazwy kolejki klienta (do czytania, serwer do niej pisze)
    pid_t pid = getpid();
    sprintf(CLIENT_QUEUE_NAME, "/%d", pid);

    mqd_t client_queue = mq_open(CLIENT_QUEUE_NAME, O_RDONLY | O_CREAT | O_EXCL, 0644, &client_queue_atr);

    if (client_queue == (mqd_t) -1) {
        perror("[KLIENT] Nie można utworzyć kolejki!\n");
        exit(EXIT_FAILURE);
    }

    // Rejestracja procedury atexit usuwającej kolejkę
    if (atexit(remove_queue) != 0) {
        if (mq_unlink(CLIENT_QUEUE_NAME) == -1) {
            perror("[KLIENT] Nie można usunąć kolejki z systemu!\n");
        }

        printf("[KLIENT] Błąd podczas rejestracji procedury atexit!\n");
        exit(EXIT_FAILURE);
    }

    // Rejestracja funkcji przechwytującej sygnał SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("[KLIENT] Błąd podczas rejestracj funckcji przechwytującej sygnał SIGINT!\n");
        exit(EXIT_FAILURE);
    }

    // Otworzenie kolejki serwera (do pisania, serwer z niej czyta)
    mqd_t server_queue = mq_open(SERVER_QUEUE, O_WRONLY);

    if (server_queue == (mqd_t) -1) {
        perror("[KLIENT] Nie można oworzyć kolejki serwera!\n");
        exit(EXIT_FAILURE);
    }

    // Główna pętla programu klienta
    char operation_buffer[MESSAGE_SIZE - 16];
    char message[MESSAGE_SIZE];
    char return_message[MESSAGE_SIZE];
    char* fgets_result;

    srand(time(NULL) + 1863);
    while (1) {
        // Wczytanie działania
        printf("Podaj działanie: ");
        fgets_result = fgets(operation_buffer, MESSAGE_SIZE - 16, stdin);

        if (fgets_result == NULL) {
            exit(EXIT_SUCCESS);
        }

        sprintf(message, "%d %s", pid, operation_buffer);

        // Wysłanie komunikatu do serwera
        if (mq_send(server_queue, message, MESSAGE_SIZE, 0) == -1) {
            perror("[KLIENT] Nie można wysłać komunikatu do serwera!\n");
            exit(EXIT_FAILURE);
        }

        // Wprowadzenie opóźnienia o losowej długości
        sleep(rand() % 3 + 1);

        // Odczytanie komunikatu z serwera
        if (mq_receive(client_queue, return_message, MESSAGE_SIZE, NULL) == -1) {
            perror("[KLIENT] Błąd podczas odczytu komunikatu z kolejki!\n");
            exit(EXIT_FAILURE);
        }

        // Wypisanie komunikatu z serwera
        printf("\nOdpowiedź serwera: %s\n\n", return_message);
    }

    exit(EXIT_SUCCESS);
}
