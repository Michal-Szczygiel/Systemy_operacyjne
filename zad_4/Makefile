CO = gcc
CFLAGS = -Wall -std=c99 -pedantic -O

NAME = zad_4

EXEC = $(NAME).x

build:
	$(CO) $(CFLAGS) $(NAME).c -o $(EXEC)

run:
	./$(EXEC)
	@echo "\n----------------------------------\nPorównanie pliku źródłowego z docelowym:"
	@diff -s data.txt output_file.txt

clean:
	rm *.x
	rm -f output_file.txt
