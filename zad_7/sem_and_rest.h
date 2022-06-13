#ifndef SEM_AND_REST_H
#define SEM_AND_REST_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

// Otwiera plik semafora, w przypadku błedu następuje przerwanie działania procesu
sem_t* open_semaphore(const char* semaphore_name, const char* process_type);

// Otwiera plik z danymi (do czytania), w przypadku błedu następuje przerwanie działania procesu
int open_data_file(const char* file_name, const char* process_type);

// Zamyka plik semafora
void close_semaphore_file(sem_t* semaphore, const char* process_type);

// Zamyka plik tekstowy
void close_text_file(int file, const char* process_type);

// Tworzy plik do którego będą zapisywane przesłane dane
int create_output_file(const char* file_name, const char* process_type);

#endif
