CO = gcc
CFLAGS = -Wall -std=c99 -pedantic -O -pthread

POWIELACZ = powielacz
PROCES = proces
SEM_LIB = sem_lib

PROGRAM_NAME = ./$(PROCES).x
PROCESSES_NUMBER = 4
SECTIONS = 3
SEMAPHOR_NAME = /semafor_4

build:
	$(CO) $(CFLAGS) $(POWIELACZ).c $(SEM_LIB).c -o $(POWIELACZ).x
	$(CO) $(CFLAGS) $(PROCES).c $(SEM_LIB).c -o $(PROCES).x

run:
	./$(POWIELACZ).x $(PROGRAM_NAME) $(PROCESSES_NUMBER) $(SECTIONS) $(SEMAPHOR_NAME)

clean:
	rm *.x
	rm *.txt
