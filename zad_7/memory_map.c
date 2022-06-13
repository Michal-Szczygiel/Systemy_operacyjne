#include "memory_map.h"

int open_shared_memory_file(const char* file_name, const char* process_type) {
    char buffer[512];

    // Otwarcie pliku z pamięcią współdzieloną
    int shared_memory = shm_open(file_name, O_RDWR, 0644);

    // Wypisanie deskryptora pliku pamięci współdzielonej
    printf("[%s] Deskryptor pliku pamieci wspoldzielonej %d\n", process_type, shared_memory);

    // Sprawdzenie poprawności otwarcia pliku z pamięcią współdzieloną
    if (shared_memory == -1) {
        sprintf(buffer, "[%s] Nie mozna otworzyc pliku pamieci wspoldzielonej!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }

    return shared_memory;
}

SegmentPD* map_file_to_segment(int shared_memory, const char* process_type) {
    char buffer[512];

    // Mapowanie pliku pamięci współdzielonej na SegmentPD
    SegmentPD* shared_buffer = (SegmentPD*) mmap(NULL, sizeof(SegmentPD), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);

    // Wypisanie adresu zmapowanej pamięci współdzielonej
    printf("[%s] Adres zmapowanej pamieci: %p\n", process_type, (void*) shared_buffer);

    // Sprawdzenie poprawności mapowania
    if (shared_buffer == MAP_FAILED) {
        sprintf(buffer, "[%s] Nie udalo sie zmapowac pliku pamiec wspldzielonej!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }

    return shared_buffer;
}

void unmap_shared_memory_file(SegmentPD* segment_ptr, const char* process_type) {
    char buffer[512];

    if (munmap((void *) segment_ptr, sizeof(SegmentPD)) == -1) {
        sprintf(buffer, "[%s] Nie udalo sie usunac mapowania pliku pamieci wspoldzielonej!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }
}

void close_shared_memory_file(int shared_memory, const char* process_type) {
    char buffer[512];

    // Zamykanie pliku pamięci współdzielonej
    if (close(shared_memory) == -1) {
        sprintf(buffer, "[%s] Nie mozna zamknac pliku pamieci wspoldzielonej!\n", process_type);
        perror(buffer);
        _exit(EXIT_FAILURE);
    }
}
