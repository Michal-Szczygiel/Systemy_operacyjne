#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <limits.h>
#include <string.h>

// Musi być większa od 16
#define MESSAGE_SIZE 256

static const char* SERVER_QUEUE = "/server_queue";

// Funkcja tworząca kolejkę dla klientów (serwer)
mqd_t create_server_queue();

// Funkcja otwierająca kolejkę klienta
mqd_t open_clien_queue(const char* client_name);

// Funkcja wysyłająca wiadomość do klienta
void send_message_co_client(mqd_t client_queue, const char* message);

// Funkcja zamykająca kolejkę klienta
void close_client_queue(mqd_t client_queue);

#endif
