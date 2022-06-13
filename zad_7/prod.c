#include "memory_map.h"
#include "sem_and_rest.h"

int main(int argc, char* argv[]) {
    // {plik z danymi} {plik pamięci współdzielonej} {plik semafora}
    if (argc != 4) {
        printf("[PRODUCENT] Blad parametrow, podaj: {nazwe pliku z danymi} {nazwe pamieci wspoldzielonej} {nazwe semafora}\n");
        _exit(EXIT_FAILURE);
    }

    // Otwarcie pliku z pamięcią współdzieloną
    int shared_memory = open_shared_memory_file(argv[2], "PRODUCENT");

    // Mapowanie pliku pamięci współdzielonej na SegmentPD
    SegmentPD* shared_buffer = map_file_to_segment(shared_memory, "PRODUCENT");

    // Otwarcie semafora
    sem_t* semaphore = open_semaphore(argv[3], "PRODUCENT");

    // Otwarcie pliku z danymi - do czytania
    int input_file = open_data_file(argv[1], "PRODUCENT");

    // Zasianie generatora liczb pesudolosowych
    srand(time(0) + 3587);

    // Utworzenie bufora na odczytywane dane
    char buffer[BUFFER_SIZE];

    // Czytanie danych z pliku
    int sended_bytes = read(input_file, buffer, BUFFER_SIZE - 1);

    // Zakończenie treści bufora zerem
    buffer[sended_bytes] = '\0';

    // Sprawdzenie poprawności odczytu z pliku
    while (sended_bytes != 0) {
        // Odczekanie 1 - 3 sekund
        sleep(rand() % 3 + 1);

        // Sprawdzenie czy dane się nie skończyły
        if (sended_bytes == 0) {
            printf("[PRODUCENT] Dane sie skonczyly!\n");

            // Początek sekcji krytycznej
            if (sem_wait(semaphore) == -1) {
                perror("[PRODUCENT] Blad opuszczania semafora!\n");
                _exit(EXIT_FAILURE);
            }

            // Ustawienie flagi końca odczytu
            shared_buffer->eof = 1;

            // Koniec sekcji krytycznej
            if (sem_post(semaphore) == -1) {
                perror("[PRODUCENT] Blad podnoszenia semafora!\n");
                _exit(EXIT_FAILURE);
            }

            break;
        }

        // Wykonywanie operacji na pamięci współdzielonej

        // Sprawdzenie czy możliwe jest wykonanie zapisu do wskazanego
        // indexu w pamięci współdzielonej, jeśli tak, przepisanie tam
        // zawartości bufora

        // Znacznik czy dostęp do indexu jest możliwy
        int is_saved = 0;

        while (is_saved == 0) {
            // Początek sekcji krytycznej
            if (sem_wait(semaphore) == -1) {
                perror("[PRODUCENT] Blad opuszczania semafora!\n");
                _exit(EXIT_FAILURE);
            }

            if ((shared_buffer->put_index + 1) % ELEMENTS != shared_buffer->get_index) {
                is_saved = 1;

                strncpy(shared_buffer->buffer[shared_buffer->put_index], buffer, BUFFER_SIZE);
                printf("[PRODUCENT] Przeslano [%d]: {%s}\n", sended_bytes, buffer);

                shared_buffer->put_index = (shared_buffer->put_index + 1) % ELEMENTS;
            }
    
            // Koniec sekcji krytycznej
            if (sem_post(semaphore) == -1) {
                perror("[PRODUCENT] Blad podnoszenia semafora!\n");
                _exit(EXIT_FAILURE);
            }

            if (is_saved == 0) {
                sleep(1);
            }
        }

        // Czytanie danych z pliku
        sended_bytes = read(input_file, buffer, BUFFER_SIZE - 1);

        // Zakończenie treści bufora zerem
        buffer[sended_bytes] = '\0';
    }

    // Początek sekcji krytycznej
    if (sem_wait(semaphore) == -1) {
        perror("[PRODUCENT] Blad opuszczania semafora!\n");
        _exit(EXIT_FAILURE);
    }

    // Ustawienie flagi końca odczytu
    shared_buffer->eof = 1;
    printf("[PRODUCENT] Dane sie skonczyly!\n");

    // Koniec sekcji krytycznej
    if (sem_post(semaphore) == -1) {
        perror("[PRODUCENT] Blad podnoszenia semafora!\n");
        _exit(EXIT_FAILURE);
    }

    // Usunięcie mapowania pliku pamięci współdzielonej
    unmap_shared_memory_file(shared_buffer, "PRODUCENT");

    // Zamykanie pliku pamięci współdzielonej
    close_shared_memory_file(shared_memory, "PRODUCENT");

    // Zamykanie semafora
    close_semaphore_file(semaphore, "PRODUCENT");

    // Zamykanie pliku z danymi
    close_text_file(input_file, "PRODUCENT");

    return EXIT_SUCCESS;
}
