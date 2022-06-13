#define _REENTRANT

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define SECTIONS 8
#define THREADS 4

// Struktura do przekazywania parametrów do wątku
typedef struct {
    int sections_number;
    int thread_number;
} ThreadArgs;

// Mutex jako zmienna globalna
static pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

static int GLOBAL_COUNTER = 0;

void* thread_function(void* _args) {
    ThreadArgs* args = (ThreadArgs*) _args;
    int private_counter;

    for (int i = 0; i < args->sections_number; i++) {
        sleep((rand() + args->thread_number * 71343) % 3 + 1);

        printf("[WĄTEK %d] Komunikat z sekcji prywatnej!\n", args->thread_number);

        // Wejście do sekcji krytycznej
        if (pthread_mutex_lock(&MUTEX) != 0) {
            printf("[WĄTEK %d] Błąd podczas zamykania mutexu!\n", args->thread_number);
            pthread_exit(NULL);
        }

        private_counter = GLOBAL_COUNTER;
        private_counter += 1;

        sleep((rand() + args->thread_number * 71343) % 3 + 1);

        printf("\033[50C[WĄTEK %d] Komunikat z sekcji krytycznej!\n", args->thread_number);
        GLOBAL_COUNTER = private_counter;

        // Wyjście z sekcji krytycznej
        if (pthread_mutex_unlock(&MUTEX) != 0) {
            printf("[WĄTEK %d] Błąd podczas otwierania mutexu!\n", args->thread_number);
            pthread_exit(NULL);
        }
    }
    
    return NULL;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    // Przygotowanie struktur z argumentami dla wątków
    static ThreadArgs args[THREADS];

    for (int i = 0; i < THREADS; i++) {
        args[i].sections_number = SECTIONS;
    }

    // Przygotowanie tablicy pod ID wątków
    pthread_t threads[THREADS];

    // Tworzenie nowych wątków
    for (int i = 0; i < THREADS; i++) {
        args[i].thread_number = i;

        if (pthread_create(&threads[i], NULL, thread_function, (void*) &args[i]) != 0) {
            printf("Nie można utworzyć nowego wątku!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Oczekiwanie na zakończenie działania wątków
    for (int i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Błąd podczas oczekiwania na zakończenie wątku!\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\n\nStatus licznika: %d (wartość oczekiwana: %d)\n", GLOBAL_COUNTER, THREADS * SECTIONS);

    return EXIT_SUCCESS;
}
