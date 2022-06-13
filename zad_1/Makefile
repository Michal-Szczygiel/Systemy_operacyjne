CO = gcc
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC

NAME_A = zad_1a
NAME_B = zad_1b
NAME_C = zad_1c
NAME_D = zad_1d
NAME_E = zad_1e

EXEC_A = $(NAME_A).x
EXEC_B = $(NAME_B).x
EXEC_C = $(NAME_C).x
EXEC_D = $(NAME_D).x
EXEC_E = $(NAME_E).x

$(EXEC_A):
	$(CO) $(CFLAGS) $(NAME_A).c -o $(EXEC_A)

$(EXEC_B):
	$(CO) $(CFLAGS) $(NAME_B).c -o $(EXEC_B)

$(EXEC_C):
	$(CO) $(CFLAGS) $(NAME_C).c -o $(EXEC_C)

$(EXEC_D):
	$(CO) $(CFLAGS) $(NAME_D).c -o $(EXEC_D)

$(EXEC_E):
	$(CO) $(CFLAGS) $(NAME_E).c -o $(EXEC_E)


run_1a: $(EXEC_A)
	./$(EXEC_A)

run_1b: $(EXEC_B)
	./$(EXEC_B)

run_1c: $(EXEC_C)
	./$(EXEC_C)

run_1d: $(EXEC_D)
	./$(EXEC_D)

run_1e: $(EXEC_E)
	./$(EXEC_E)

clean:
	rm *.x
