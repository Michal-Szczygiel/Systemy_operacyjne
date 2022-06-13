#include <sys/wait.h>
#include <signal.h>

#include "memory_map.h"
#include "sem_and_rest.h"

// Nazwa pliku pamięci współdzielonej
static char* shared_memory_file_name;

// Nazwa semafora
static char* semaphore_name;

// Funkcja atexit usuwająca plik pamięci współdzielonej
void remove_shared_memory() {
    if (shm_unlink(shared_memory_file_name) == -1) {
        perror("[PROCES MACIERZYSTY] Blad podczas usuwania pliku pamieci wspoldzielonej!\n");
    }
}

// Funkcja atexit usuwająca semafor
void remove_semaphore() {
    if (sem_unlink(semaphore_name) == -1) {
        perror("[PROCES MACIERZYSTY] Blad podczas usuwania semafora!\n");
    }
}

// Funkcja obsługi sygnału
void signal_handler(int signal) {
    printf("[PROCES MACIERZYSTY] Przychwycono sygnal SIGINT!\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    // {plik z danymi} {plik na dane} {plik pamięci współdzielonej} {semafor}
    if (argc != 5) {
        printf("[PROCES MACIERZYSTY] Blad parametrow, podaj: {nazwe pliku z danymi} {nazwe pliku na dane} {nazwe pliku pamieci wspoldzielonej} {nazwe semafora}\n");
        exit(EXIT_FAILURE);
    }

    // Przypisanie nazw plików do wskażników statycznych
    shared_memory_file_name = argv[3];
    semaphore_name = argv[4];

    // Tworzenie pliku pamięci współdzielonej
    int shared_memory = shm_open(shared_memory_file_name, O_RDWR | O_CREAT | O_EXCL, 0644);

    // Sprawdzanie czy plik pamięci współdzielonej został utworzony
    if (shared_memory == -1) {
        // Błąd podczas tworzenia pliku pamięci współdzielonej
        perror("[PROCES MACIERZYSTY] Nie mozna utworzyc pliku pamieci wspoldzielonej!\n");
        exit(EXIT_FAILURE);
    }

    // Wypisanie deskryptora pamięci współdzielonej
    printf("[PROCES MACIERZYSTY] Deskryptor pliku pamieci wspoldzielonej: %d\n", shared_memory);

    // Rejestracja pierwszej funkcji atexit - usuwa plik pamięci współdzielonej
    if (atexit(remove_shared_memory) != 0) {
        // Błąd podczas resjestrowania funkcji atexit
        printf("[PROCES MACIERZYSTY] Nie udalo sie zarejestrowac pierwszej funkcji 'atexit'!\n");

        // Ręczne usunięcie pliku pamięci współdzielonej w przypadku błędu rejestracji atexit
        if (shm_unlink(shared_memory_file_name) == -1) {
            perror("[PROCES MACIERZYSTY] Blad podczas usuwania pliku pamieci wspoldzielonej!\n");
        }
        exit(EXIT_FAILURE);
    }

    // Tworzenie nowego semafora
    sem_t* semaphore = sem_open(semaphore_name, O_CREAT | O_EXCL, 0664, 1);

    // Sprawdzenie czy semafor został utworzony poprawnie
    if (semaphore == SEM_FAILED) {
        // Błąd podczas tworzenia nowego semafora
        perror("[PROCES MACIERZYSTY] Blad podczas tworzenia semafora!\n");

        // Wywołanie poprzednio zarejestrowanej funkcji atexit
        exit(EXIT_FAILURE);
    }

    // Wypisanie adresu semafora
    printf("[PROCES MACIERZYSTY] Adres semafora: %p\n", (void*) semaphore);

    // Rejestracja drugiej funkcji atexit - uwuwa semafor
    if (atexit(remove_semaphore) == -1) {
        // Błąd podczas rejestrowania funkcji atexit
        printf("[PROCES MACIERZYSTY] Nie udalo sie zarejestrowac drugiej funkcji 'atexit'!\n");

        // Ręczne usunięcie semafora, plik pamięci współdzielonej jest uwuwany przez wcześniej
        // zarejestrowaną funkcją atexit
        if (sem_unlink(semaphore_name) == -1) {
            perror("[PROCES MACIERZYSTY] Blad podczas usuwania semafora!\n");
        }

        // Wywołanie poprzedniej funkcji atexit
        exit(EXIT_FAILURE);
    }

    // Zamykanie semafora - w tym procesie nie jest potrzebny
    if (sem_close(semaphore) == -1) {
        perror("[PROCES MACIERZYSTY] Nie mozna zamknac semafora!\n");
        exit(EXIT_FAILURE);
    }

    // Rejestracja funcji przechwytującej sygnał
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("[PROCES MACIERZYSTY] Blad rejestracji obslugi sygnalu!\n");
        exit(EXIT_FAILURE);
    }

    // Ustawianie rozmiaru pamięci współdzielonej - wielkość struktury SegmentPD
    if (ftruncate(shared_memory, sizeof(SegmentPD)) == -1) {
        // Błąd podczas ustawiania rozmiaru pamięci współdzielonej
        perror("[PROCES MACIERZYSTY] Nie udalo sie ustawic rozmiaru pamieci wspoldzielonej!\n");
        exit(EXIT_FAILURE);
    }

    // Inicjalizacja indeksów bufora
    // Mapowanie pliku pamięci współdzielonej
    SegmentPD* shared_buffer = (SegmentPD*) mmap(NULL, sizeof(SegmentPD), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);

    // Sprawdzenie poprawności mapowania
    if (shared_buffer == MAP_FAILED) {
        perror("[PROCES MACIERZYSTY] Nie udalo sie zmapowac pliku pamiec wspldzielonej!\n");
        exit(EXIT_FAILURE);
    }

    // Zerowanie indeksów i eof
    shared_buffer->put_index = 0;
    shared_buffer->get_index = 0;
    shared_buffer->eof = 0;

    // Usunięcie mapowania pliku pamięci współdzielonej
    if (munmap(shared_buffer, sizeof(SegmentPD)) == -1) {
        perror("[PROCES MACIERZYSTY] Nie udalo sie usunac mapowania pliku pamieci wspoldzielonej!\n");
        exit(EXIT_FAILURE);
    }

    // Zamykanie pliku pamięci współdzielonej - w tym procesie nie jest potrzebna
    if (close(shared_memory) == -1) {
        // Błąd podczas zamykania pliku pamięci współdzielonej
        perror("[PROCES MACIERZYSTY] Blad podczas zamykania pliku pamieci wspoldzielonej!\n");
        exit(EXIT_FAILURE);
    }

    // Tworzenie procesów potomnych
    // Proces producenta
    pid_t prod_pid = fork();
    if (prod_pid == 0) {
        // Proces potomny - producent
        // {plik z danymi} {plik pamięci wspódzielonej} {plik semafora}
        if (execlp("./prod.x", "./prod.x", argv[1], argv[3], argv[4], NULL) == -1) {
            // Błąd podczas uruchamiania procesu producenta
            perror("[PROCES POTOMNY] Blad podczas uruchamiania procesu producenta!\n");
            _exit(EXIT_FAILURE);
        }
    } else if (prod_pid == -1) {
        // Błąd tworzenia procesu potomnego
        perror("[PROCES MACIERZYSTY] Blad tworzenia procesu potomnego!\n");
        exit(EXIT_FAILURE);
    }

    // Proces konsumenta
    pid_t cons_pid = fork();
    if (cons_pid == 0) {
        // Proces potomny - konsument
        // {plik na dane} {plik pamięci współdzielonej} {plik semafora}
        if (execlp("./cons.x", "./cons.x", argv[2], argv[3], argv[4], NULL) == -1) {
            // Błąd podczas uruchamiania procesu konsumenta
            perror("[PROCES POTOMNY] Blad podczas uruchamiania procesu konsumenta!\n");
            _exit(EXIT_FAILURE);
        }
    } else if (cons_pid == -1) {
        // Błąd tworzenia procesu potomnego
        perror("[PROCES MACIERZYSTY] Blad tworzenia procesu potomnego!\n");
        exit(EXIT_FAILURE);
    }

    // Oczekiwanie na pierwszy proces potomny
    if (wait(NULL) == -1) {
        // Błąd podczas oczekiwania na proces potomny
        perror("[PROCES MACIERZYSTY] Blad podczas oczekiwania na proces potomny!\n");
        exit(EXIT_FAILURE);
    }

    printf("[PROCES MACIERZYSTY] Zamknieto pierwszy proces potomny!\n");

    // Oczekiwanie na drugi proces potomny
    if (wait(NULL) == -1) {
        // Błąd podczas oczekiwania na proces potomny
        perror("[PROCES MACIERZYSTY] Blad podczas oczekiwania na proces potomny!\n");
        exit(EXIT_FAILURE);
    }

    printf("[PROCES MACIERZYSTY] Zamknieto drugi proces potomny!\n");

    exit(EXIT_SUCCESS);
}
