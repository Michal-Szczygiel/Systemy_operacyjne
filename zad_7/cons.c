#include "memory_map.h"
#include "sem_and_rest.h"

int main(int argc, char* argv[]) {
    // {plik na dane} {plik pamięci współdzielonej} {plik semafora}
    if (argc != 4) {
        printf("[KONSUMENT] Blad parametrow, podaj: {nazwe pliku z danymi} {nazwe pliku pamieci wspoldzielonej} {nazwe semafora}\n");
        _exit(EXIT_FAILURE);
    }

    // Otwarcie pliku z pamięcią współdzieloną
    int shared_memory = open_shared_memory_file(argv[2], "KONSUMENT");

    // Mapowanie pliku pamięci współdzielonej na SegmentPD
    SegmentPD* shared_buffer = map_file_to_segment(shared_memory, "KONSUMENT");

    // Otwarcie semafora
    sem_t* semaphore = open_semaphore(argv[3], "KONSUMENT");

    // Otwarcie pliku na przesłane dane - po pisania
    int output_file = create_output_file(argv[1], "KONSUMENT");

    // Zasianie generatora liczb pesudolosowych
    srand(time(0) + 8753);

    // Odczytywanie danych z segmentu pamięci współdzielonej i zapisywanie ich do pliku
    while (1) {
        // Losowe opóźnienie
        sleep(rand() % 3 + 1);

        // Początek sekcji krytycznej
        if (sem_wait(semaphore) == -1) {
            perror("[KONSUMENT] Blad opuszczania semafora!\n");
            _exit(EXIT_FAILURE);
        }

        // Sprawdzenie czy nie osiągnięto warunku zakończenia wypisywania
        if (shared_buffer->eof == 1 && shared_buffer->get_index == shared_buffer->put_index) {
            break;
        }

        if (shared_buffer->get_index != shared_buffer->put_index) {
            if (write(output_file, shared_buffer->buffer[shared_buffer->get_index], strlen(shared_buffer->buffer[shared_buffer->get_index])) == -1) {
                perror("[KONSUMENT] Blad zapisu do pliku!\n");
                _exit(EXIT_FAILURE);
            }

            printf("[KONSUMENT] Odebrano [%lu]: {%s}\n", strlen(shared_buffer->buffer[shared_buffer->get_index]), shared_buffer->buffer[shared_buffer->get_index]);

            shared_buffer->get_index = (shared_buffer->get_index + 1) % ELEMENTS;
        }

        // Koniec sekcji krytycznej
        if (sem_post(semaphore) == -1) {
            perror("[KONSUMENT] Blad podnoszenia semafora!\n");
            _exit(EXIT_FAILURE);
        }

    }

    // Usunięcie mapowania pliku pamięci współdzielonej
    unmap_shared_memory_file(shared_buffer, "KONSUMENT");

    // Zamykanie pliku pamięci współdzielonej
    close_shared_memory_file(shared_memory, "KONSUMENT");

    // Zamykanie semafora
    close_semaphore_file(semaphore, "KONSUMENT");

    // Zamykanie pliku na przesłane dane
    close_text_file(output_file, "KONSUMENT");

    return EXIT_SUCCESS;
}
