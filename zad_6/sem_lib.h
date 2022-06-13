#ifndef SEM_LIB
#define SEM_LIB

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
//#include <fcntl.h>
#include <sys/stat.h>

#define PERROR_BUFFER 256

// Funkcja otwierająca semafor (jeśli semafor o danej nazwie nie istnieje to jest tworzony)
// W przypadku wystąpienia błędu funkcja kończy działanie procesu funkcją exit oraz komunikatem o błędzie
sem_t *open_semaphore_parent(const char *name, int flag, int access, int init_value);

// Funkcja zamykająca semafor, w przypadku niepowodzenia funcja kończy proces funkcją exit oraz
// komunikatem o błędzie
void close_semaphore_parent(sem_t *semaphore);

// Podobnie jak powyżej tylko dla procesu potomnego
sem_t *open_semaphore_child(const char *name, int flag, int access, int init_value, int process_number);

// Podobnie jak powyżej tylko dla procesu potomnego
void close_semaphore_child(sem_t *semaphore, int process_number);

// Opuszanie semafora, w przypadku błędu wywoływana jest funkcja exit oraz wyśietlany jest komunikat o błędzie
void wait_semaphore_child(sem_t *semaphore, int process_number);

// Podnoszenie semafora, w przypadku błędu wywoływana jest funkcja exit oraz wyśietlany jest komunikat o błędzie
void post_semaphore_child(sem_t *semaphore, int process_number);

// Wczytuje liczbę z pliku, inkrementuje ją i zapisuje spowrotem do pliku.
// w przypadku błędu wywoływana jest funkcja exit oraz wyśietlany jest komunikat o błędzie
void increment_number_from_file_child(const char* file_name, int process_number);

// Tworzy plik z zerem, w przypadku błędu wywoływana jest funkcja exit oraz wyśietlany jest komunikat o błędzie
void initialize_number_file_parent(const char* file_name);

// Otwiera plik, wcztuje liczbę i sprawdza czy jest ona zgodna z przewidywaniami,
// w przypadku błędu wywoływana jest funkcja exit oraz wyśietlany jest komunikat o błędzie
void compare_number_from_file(const char* file_name, int processes_number, int sections);

#endif
