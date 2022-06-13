CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -O

SERVER = server
CLIENT = client
UTILITY = utility

build:
	$(CC) $(CFLAGS) $(SERVER).c $(UTILITY).c -o $(SERVER).x -lrt
	$(CC) $(CFLAGS) $(CLIENT).c $(UTILITY).c -o $(CLIENT).x -lrt

clean:
	rm *.x
