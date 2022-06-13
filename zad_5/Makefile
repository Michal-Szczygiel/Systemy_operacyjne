CO = gcc
CFLAGS = -Wall -std=c99 -pedantic -O

PROGRAM = zad_5
PROD = prod
CONS = cons

DATA_FILE = data_file.txt
OUTPUT_FILE = output_file.txt
FIFO = fifo_example

build:
	$(CO) $(CFLAGS) $(PROGRAM).c -o $(PROGRAM).x
	$(CO) $(CFLAGS) $(PROD).c -o $(PROD).x
	$(CO) $(CFLAGS) $(CONS).c -o $(CONS).x

run:
	./$(PROGRAM).x $(FIFO) ./$(PROD).x ./$(CONS).x $(DATA_FILE) $(OUTPUT_FILE)
	@echo "\n----------------------------------\nPorównanie pliku źródłowego z docelowym:"
	@diff -s $(DATA_FILE) $(OUTPUT_FILE)

clean:
	rm *.x
	rm -f $(OUTPUT_FILE)
