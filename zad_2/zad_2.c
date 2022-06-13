#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void print_summary() {
    uid_t uid = getuid();
    gid_t gid = getgid();
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = __getpgid(0);

    printf("---------------------\nUID: %u\nGID: %u\nPID: %d\nPPID: %d\nPGID: %d\n---------------------\n\n", uid, gid, pid, ppid, pgid);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Blad argumentow! Podaj sciezke do programu.\n");
        exit(EXIT_FAILURE);
    }

    printf("Proces macierzysty:\n");
    print_summary();

    for (int i = 0; i < 3; i++) {
        switch (fork()) {
            case -1:
                perror("Nie mozna utworzyc procesu potomnego!\n");
                _exit(EXIT_FAILURE);

            case 0:
                printf("Proces potomny:\n");
                if (execlp(argv[1], argv[1], NULL) == -1) {
                    perror("execlp error!\n");
                }
                _exit(2);

            default:
                wait(NULL);
        }
    }

    return 0;
}
