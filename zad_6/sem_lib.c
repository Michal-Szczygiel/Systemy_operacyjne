#include "sem_lib.h"

sem_t *open_semaphore_parent(const char *name, int flag, int access, int init_value) {
    sem_t *semaphore = sem_open(name, flag, access, init_value);

    if (semaphore == SEM_FAILED) {
        perror("[PROCES MACIERZYSTY] Nie mozna utworzyc semafora!\n");
        exit(EXIT_FAILURE);
    }

    return semaphore;
}

void close_semaphore_parent(sem_t *semaphore) {
    if (sem_close(semaphore) == -1) {
        perror("[PROCES MACIERZYSTY] Nie mozna zamknac semafora!\n");
        exit(EXIT_FAILURE);
    }
}

sem_t *open_semaphore_child(const char *name, int flag, int access, int init_value, int process_number) {
    sem_t *semaphore = sem_open(name, flag, access, init_value);
    char perror_buffer[PERROR_BUFFER];

    if (semaphore == SEM_FAILED) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Nie mozna otworzyc semafora!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }

    return semaphore;
}

void close_semaphore_child(sem_t *semaphore, int process_number) {
    char perror_buffer[PERROR_BUFFER];

    if (sem_close(semaphore) == -1) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Nie mozna zamknac semafora!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }
}

void wait_semaphore_child(sem_t *semaphore, int process_number) {
    char perror_buffer[PERROR_BUFFER];

    if (sem_wait(semaphore) == -1) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Problem z opuszczaniem semafora!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }
}

void post_semaphore_child(sem_t *semaphore, int process_number) {
    char perror_buffer[PERROR_BUFFER];

    if (sem_post(semaphore) == -1) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Problem z podnoszeniem semafora!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }
}

void increment_number_from_file_child(const char* file_name, int process_number) {
    FILE *number_file = fopen("number.txt", "r+");
    char perror_buffer[PERROR_BUFFER];
    int number_from_file;

    if (number_file == NULL) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Nie udalo sie otworzyc pliku 'number.txt'!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }

    // Wczytanie zawartości pliku
    if (fscanf(number_file, "%d", &number_from_file) != 1) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Problem z wczytaniem zawartosci pliku 'number.txt'!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }
    sleep(rand() % 3 + 1);
    number_from_file += 1;

    // Zapisanie liczby do pliku
    fseek(number_file, 0, SEEK_SET);
    if (fprintf(number_file, "%d", number_from_file) < 0) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Problem z zapisem pliku 'number.txt'!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }

    // Zamknięcie pliku 'number.txt'
    if (fclose(number_file) != 0) {
        sprintf(perror_buffer, "[PROCES POTOMNY %d] Problem z zamknieciem pliku 'number.txt'!\n", process_number);
        perror(perror_buffer);
        _exit(EXIT_FAILURE);
    }
}

void initialize_number_file_parent(const char* file_name) {
    // Utworzenie pliku 'number.txt'
    FILE *number_file = fopen(file_name, "w");
    if (number_file == NULL) {
        perror("[PROCES MACIERZYSTY] Nie mozna utworzyc pliku 'number.txt'!\n");
        exit(EXIT_FAILURE);
    }

    // Wpisanie 0 do pliku
    if (fprintf(number_file, "0") < 0) {
        perror("[PROCES MACIERZYSTY] Nie mozna zapisac do pliku 'number.txt'!\n");
        exit(EXIT_FAILURE);
    }

    // Zamknięcie pliku number.txt
    if (fclose(number_file) == EOF) {
        perror("[PROCES MACIERZYSTY] Nie mozna zamknac pliku 'number.txt'!\n");
        exit(EXIT_FAILURE);
    }
}

void compare_number_from_file(const char* file_name, int processes_number, int sections) {
    FILE *number_file = fopen(file_name, "r");
    int result;

    if (number_file == NULL) {
        perror("[PROCES MACIERZYSTY] Nie mozna otworzyc pliku 'number.txt'!\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(number_file, "%d", &result) != 1) {
        perror("[PROCES MACIERZYSTY] Problem z wczytaniem zawartosci pliku 'number.txt'!\n");
        exit(EXIT_FAILURE);
    }

    // Zamknięcie pliku number.txt
    if (fclose(number_file) == EOF) {
        perror("[PROCES MACIERZYSTY] Nie mozna zamknac pliku 'number.txt'!\n");
        exit(EXIT_FAILURE);
    }

    if ((processes_number * sections) == result) {
        printf("Wszystko zadzialalo jak w zegarku!\n");
    } else {
        printf("Pojawil sie jakis problem!\n");
    }
}
