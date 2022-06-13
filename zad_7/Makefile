CO = gcc
CFLAGS = -Wall -std=c99 -pedantic -O
LIBS = -pthread -lrt

INPUT_FILE = input_file.txt
OUTPUT_FILE = output_file.txt
SHARED_MEMORY = /shared_memory2
SEMAPHORE = /semaphore_2

SUPER = super
PROD = prod
CONS = cons
MEMORY_MAP = memory_map
SEM_AND_REST = sem_and_rest

LIBRARY = utillibrary

build_static:
	$(CO) $(CFLAGS) -c $(MEMORY_MAP).c $(SEM_AND_REST).c
	ar rcs lib$(LIBRARY).a $(MEMORY_MAP).o $(SEM_AND_REST).o

	$(CO) $(CFLAGS) $(SUPER).c -o $(SUPER).x $(LIBS)
	$(CO) $(CFLAGS) $(PROD).c -o $(PROD).x -L. -l$(LIBRARY) $(LIBS)
	$(CO) $(CFLAGS) $(CONS).c -o $(CONS).x -L. -l$(LIBRARY) $(LIBS)

build_dynamic:
	$(CO) $(CFLAGS) -shared $(MEMORY_MAP).c $(SEM_AND_REST).c -o lib$(LIBRARY).so
	LD_LIBRARY_PATH=/home/michal/systemy_operacyjne/zad_7/
	export LD_LIBRARY_PATH

	$(CO) $(CFLAGS) $(SUPER).c -o $(SUPER).x $(LIBS)
	$(CO) $(CFLAGS) $(PROD).c -o $(PROD).x -L. -l$(LIBRARY) $(LIBS)
	$(CO) $(CFLAGS) $(CONS).c -o $(CONS).x -L. -l$(LIBRARY) $(LIBS)

run:
	./$(SUPER).x $(INPUT_FILE) $(OUTPUT_FILE) $(SHARED_MEMORY) $(SEMAPHORE)
	diff -s $(INPUT_FILE) $(OUTPUT_FILE)

clean_static:
	rm *.x
	rm *.o 
	rm *.a
	rm $(OUTPUT_FILE)

clean_dynamic:
	rm *.x
	rm *.so
	rm $(OUTPUT_FILE)
