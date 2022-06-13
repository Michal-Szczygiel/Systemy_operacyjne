#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 64
#define PRINT_BUFFER 256

int main() {
    char input_buffer[BUFFER_SIZE + 1], output_buffer[BUFFER_SIZE + 1], print_buffer[PRINT_BUFFER];
    int filedes[2], input_file, output_file, sended_bytes, received_bytes;

    if (pipe(filedes) == -1) {
        perror("Nie mozna utworzyc potoku!\n");
        exit(EXIT_FAILURE);
    }

    switch (fork()) {
        case -1:
            perror("Nie mozna utworzyc procesu potomnego!\n");
            exit(EXIT_FAILURE);

        case 0:
            srand(time(0) + 2765);
            output_file = open("output_file.txt", O_WRONLY | O_CREAT, 777);

            if (output_file == -1) {
                perror("Nie mozna stworzyc pliku na dane!\n");
                exit(EXIT_FAILURE);
            }

            if (close(filedes[1]) == -1) {
                perror("Blad podczas zamykania strumienia!\n");
                exit(EXIT_FAILURE);
            }

            received_bytes = read(filedes[0], output_buffer, BUFFER_SIZE);

            while (received_bytes != 0) {
                if (received_bytes == 0) {
                    if (write(STDOUT_FILENO, "Dane sie skonczyly!\n", 21) == -1) {
                        perror("Blad wypisywania komunikatu!\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                sleep(rand() % 3 + 1);
                output_buffer[received_bytes] = '\0';
                sprintf(print_buffer, "Otrzymano %d bajtow. Tresc: {%s}\n", received_bytes, output_buffer);

                if (write(STDOUT_FILENO, print_buffer, strlen(print_buffer)) == -1) {
                    perror("Blad wypisywania komunikatu!\n");
                    exit(EXIT_FAILURE);
                }

                if (write(output_file, output_buffer, received_bytes) == -1) {
                    perror("Blad podczas zapisu do pliku!\n");
                    exit(EXIT_FAILURE);
                }

                received_bytes = read(filedes[0], output_buffer, BUFFER_SIZE);
            }

            if (close(output_file) == -1) {
                perror("Blad podczas zamykania pliku!\n");
                exit(EXIT_FAILURE);
            }

            if (write(STDOUT_FILENO, "Zamknieto plik wynikowy!\n", 26) == -1) {
                perror("Blad wypisywania komunikatu!\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);

        default:
            srand(time(0));
            input_file = open("data.txt", O_RDONLY);

            if (input_file == -1) {
                perror("Nie mozna otworzyc pliku z danymi!\n");
                exit(EXIT_FAILURE);
            }

            sended_bytes = read(input_file, input_buffer, BUFFER_SIZE);

            while (sended_bytes != -1) {
                if (sended_bytes == 0) {
                    if (write(STDOUT_FILENO, "Dane sie skonczyly!\n", 21) == -1) {
                        perror("Blad wypisywania komunikatu!\n");
                        exit(EXIT_FAILURE);
                    }

                    break;
                }

                sleep(rand() % 3 + 1);
                input_buffer[sended_bytes] = '\0';
                sprintf(print_buffer, "Wyslano %d bajtow. Tresc: {%s}\n", sended_bytes, input_buffer);

                if (write(STDOUT_FILENO, print_buffer, strlen(print_buffer)) == -1) {
                    perror("Blad wypisywania komunikatu!\n");
                    exit(EXIT_FAILURE);
                }

                if (write(filedes[1], input_buffer, sended_bytes) == -1) {
                    perror("Blad podczas wysylania danych potokiem!\n");
                    exit(EXIT_FAILURE);
                }

                sended_bytes = read(input_file, input_buffer, BUFFER_SIZE);
            }

            if (close(filedes[1]) == -1) {
                perror("Blad podczas zamykania potoku!\n");
                exit(EXIT_FAILURE);
            }

            if (sended_bytes == -1) {
                perror("Blad odczytu z pliku!\n");
                exit(EXIT_FAILURE);
            }

            if (close(input_file) == -1) {
                perror("Blad podczas zamykania pliku!\n");
                exit(EXIT_FAILURE);
            }

            if (wait(NULL) == -1) {
                perror("Czekanie na proces potomny nie powiodlo sie!\n");
                exit(EXIT_FAILURE);
            }
    }

    return 0;
}
