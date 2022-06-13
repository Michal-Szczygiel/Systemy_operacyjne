#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// Rozmiar elementu bufora - porcja danych
#define BUFFER_SIZE 128

// Liczba elementów w buforze
#define ELEMENTS 8

typedef struct {
    char buffer[ELEMENTS][BUFFER_SIZE]; // Bufor cykilczny
    int put_index; // Index wstawiania
    int get_index; // Index wyjmowania
    int eof; // Wskaźnik zakończenia odczytu z pliku
} SegmentPD;

// Otwiera plik z pamięcią współdzieloną, w przypadku błedu następuje przerwanie działania procesu
int open_shared_memory_file(const char* file_name, const char* process_type);

// Wykonuje mapowanie otwartego pliku pamięci współdzielonej, w przypadku błędu następuje przerwanie
// działania procesu
SegmentPD* map_file_to_segment(int shared_memory, const char* process_type);

// Usuwa mapowanie pliku pamięci współdzielonej, w przypadku błedu następuje przerwanie działania procesu
void unmap_shared_memory_file(SegmentPD* segment_ptr, const char* process_type);

// Zamyka plik pamięci współdzielonej
void close_shared_memory_file(int shared_memory, const char* process_type);

#endif
