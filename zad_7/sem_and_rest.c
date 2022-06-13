#include "sem_and_rest.h"

sem_t* open_semaphore(const char* semaphore_name, const char* process_type) {
    char buffer[512];

    // Otwarcie semafora
    sem_t* semaphore = sem_open(semaphore_name, O_CREAT, 0664, 1);

    // Wypisanie adresu semafora
    printf("[%s] Adres semafora: %p\n", process_type, (void*) semaphore);

    // Sprawdzenie poprawności otwarcia semafora
    if (semaphore == SEM_FAILED) {
        sprintf(buffer, "[%s] Nie mozna otworzyc semafora!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }

    return semaphore;
}

int open_data_file(const char* file_name, const char* process_type) {
    char buffer[512];

    // Otwarcie pliku z danymi - do czytania
    int data_file = open(file_name, O_RDONLY);

    // Sprawdzenie poprawności otwarcia pliku z danymi
    if (data_file == -1) {
        sprintf(buffer, "[%s] Blad otwarcia pliku z danymi!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }

    return data_file;
}

void close_semaphore_file(sem_t* semaphore, const char* process_type) {
    char buffer[512];

    if (sem_close(semaphore) == -1) {
        sprintf(buffer, "[%s] Nie mozna zamknac semafora!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }
}

void close_text_file(int file, const char* process_type) {
    char buffer[512];

    if (close(file) == -1) {
        sprintf(buffer, "[%s] Blad podczas zamykania pliku z danymi!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }
}

int create_output_file(const char* file_name, const char* process_type) {
    char buffer[512];

    // Otwarcie pliku na przesłane dane - po pisania
    int output_file = open(file_name, O_WRONLY | O_CREAT, 0777);

    // Sprawdzenie poprawności otwarcia pliku na przesłane dane
    if (output_file == -1) {
        sprintf(buffer, "[%s] Blad otwarcia pliku na przeslane dane!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }

    return output_file;
}
