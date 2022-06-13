CO = gcc
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC

NAME_2 = zad_2
NAME_PROG = zad_1a

EXEC_2 = $(NAME_2).x
EXEC_PROG = $(NAME_PROG).x

$(EXEC_2):
	$(CO) $(CFLAGS) $(NAME_2).c -o $(EXEC_2)

$(EXEC_PROG):
	$(CO) $(CFLAGS) $(NAME_PROG).c -o $(EXEC_PROG)

run_2: $(EXEC_2) $(EXEC_PROG)
	./$(EXEC_2) ./$(EXEC_PROG)

clean:
	rm *.x
