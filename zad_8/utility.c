#include "utility.h"

mqd_t create_server_queue() {
    struct mq_attr server_queue_atr = {
        .mq_flags = 0,
        .mq_maxmsg = 8,
        .mq_msgsize = MESSAGE_SIZE,
        .mq_curmsgs = 0
    };

    mqd_t server_queue = mq_open(SERVER_QUEUE, O_RDONLY | O_CREAT | O_EXCL, 0664, &server_queue_atr);

    if (server_queue == (mqd_t) -1) {
        perror("[SERWER] Nie można utworzyć kolejki!\n");
        exit(EXIT_FAILURE);
    }

    return server_queue;
}

mqd_t open_clien_queue(const char* client_name) {
    mqd_t client_queue = mq_open(client_name, O_WRONLY);

    if (client_queue == (mqd_t) -1) {
        perror("[SERWER] Nie można otworzyć kolejki klienta!\n");
        exit(EXIT_FAILURE);
    }

    return client_queue;
}

void send_message_co_client(mqd_t client_queue, const char* message) {
    if (mq_send(client_queue, message, MESSAGE_SIZE, 0) == -1) {
        perror("[SERWER] Nie można wysłać odpowiedzi do klienta!\n");
        exit(EXIT_FAILURE);
    }
}

void close_client_queue(mqd_t client_queue) {
    if (mq_close(client_queue) == -1) {
        perror("[SERWER] Nie można zamknąć kolejki klienta!\n");
        exit(EXIT_FAILURE);
    }
}
