CO = gcc
CFLAGS = -Wall -std=c99 -pedantic -O

NAME_A = zad_3a
NAME_B = zad_3b
NAME_C = zad_3c
NAME_PROG = program

EXEC_A = $(NAME_A).x
EXEC_B = $(NAME_B).x
EXEC_C = $(NAME_C).x
EXEC_PROG = $(NAME_PROG).x

all:
	$(CO) $(CFLAGS) $(NAME_A).c -o $(EXEC_A)
	$(CO) $(CFLAGS) $(NAME_B).c -o $(EXEC_B)
	$(CO) $(CFLAGS) $(NAME_C).c -o $(EXEC_C)
	$(CO) $(CFLAGS) $(NAME_PROG).c -o $(EXEC_PROG)

clean:
	rm *.x
