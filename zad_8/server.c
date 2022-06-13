#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#include "utility.h"

// Funkcja obsługi sygnału SIGINT
void sigint_handler(int signal) {
    exit(EXIT_FAILURE);
}

// Procedura atexit
void remove_queue() {
    printf("\n[SERWER] Kończę działanie programu serwera!\n");

    if (mq_unlink(SERVER_QUEUE) == -1) {
        perror("[SERWER] Nie można usunąć kolejki z systemu!\n");
    } else {
        printf("[SERWER] Kolejka została usunięta!\n");
    }
}

int main() {
    // Utworzenie kolejki dla klientów (do komunikacji z serwerem)
    mqd_t server_queue = create_server_queue();

    // Rejestracja procedury atexit usuwającej kolejkę
    if (atexit(remove_queue) != 0) {
        if (mq_unlink(SERVER_QUEUE) == -1) {
            perror("[SERWER] Nie można usunąć kolejki z systemu!\n");
        }

        printf("[SERWER] Błąd podczas rejestracji procedury atexit!\n");
        exit(EXIT_FAILURE);
    }

    // Rejestracja funkcji przechwytującej sygnał SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("[SERWER] Błąd podczas rejestracj funckcji przechwytującej sygnał SIGINT!\n");
        exit(EXIT_FAILURE);
    }

    printf("[SERWER] Poprawnie uruchomiono serwer!\n");

    // Odebranie komunikatu z kolejki, wykonanie działania i odesłanie wyniku
    char message_buffer[MESSAGE_SIZE], client_pid[16], operator[16], client_queue_name[128], return_message[MESSAGE_SIZE];
    int number_a, number_b, result;
    mqd_t client_queue;

    srand(time(NULL) + 2457);
    // Pętla nieskończona
    while (1) {
        int param_number;

        // Odebranie komunikatu z kolejki
        if (mq_receive(server_queue, message_buffer, MESSAGE_SIZE, NULL) == -1) {
            perror("[SERWER] Błąd podczas odczytu komunikatu z kolejki!\n");
            exit(EXIT_FAILURE);
        } else {
            // Parsowanie komunikatu
            param_number = sscanf(message_buffer, "%s %d %s %d", client_pid, &number_a, operator, &number_b);

            if (param_number == 4) {
                sprintf(client_queue_name, "/%s", client_pid);
                printf("[SERWER] Klient: %s, zapytanie: %s", client_pid, message_buffer);
            }
        }

        // Wykonanie odpowiedniej operacji matematycznej
        if (strlen(operator) == 1 && param_number == 4) {
            switch (operator[0]) {
                case '+':
                    result = number_a + number_b;
                    break;
                case '-':
                    result = number_a - number_b;
                    break;
                case '*':
                    result = number_a * number_b;
                    break;
                case '/':
                    result = number_a / number_b;
                    break;
                default:
                    printf("[SERWER] Błędny operator w zapytaniu klienta: %s!\n", client_pid);
                    result = INT_MIN;
            }
        } else {
            printf("[SERWER] Błąd w zapytaniu klienta: %s!\n", client_pid);
            result = INT_MIN;
        }

        // Utworzenie odpowiedzi
        if (result != INT_MIN && param_number == 4) {
            sprintf(return_message, "%d", result);
        } else {
            sprintf(return_message, "Error!");
        }

        printf("[SERWER] Odpowiedź do klienta %s: %s\n", client_pid, return_message);

        // Wprowadzenie opóźnienia o losowej długości
        sleep(rand() % 3 + 1);

        // Otworzenie kolejki klienta
        client_queue = open_clien_queue(client_queue_name);

        // Wysłanie wiadomości z odpowiedzią do klienta
        send_message_co_client(client_queue, return_message);

        // Zamknięcie kolejki klienta
        close_client_queue(client_queue);

        printf("[SERWER] Wiadomość została wysłana do klienta %s!\n", client_pid);
    }

    exit(EXIT_SUCCESS);
}
