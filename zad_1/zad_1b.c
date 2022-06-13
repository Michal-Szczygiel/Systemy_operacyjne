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
    pid_t pgid = getpgid(0);

    if (pgid == -1) {
        perror("Nie mozna uzyskac identyfikatora PGID!\n");
        exit(EXIT_FAILURE);
    }

    printf("---------------------\nUID: %u\nGID: %u\nPID: %d\nPPID: %d\nPGID: %d\n---------------------\n\n", uid, gid, pid, ppid, pgid);
}

int main() {
    printf("Proces macierzysty:\n");
    print_summary();

    for (int i = 0; i < 3; i++) {
        switch (fork()) {
            case -1:
                perror("Nie mozna utworzyc procesu potomnego!\n");
                exit(EXIT_FAILURE);

            case 0:
                printf("Proces potomny:\n");
                print_summary();
                break;

            default:
                if (wait(NULL) == -1) {
                    perror("Czekanie na proces potomny nie powiodlo sie!\n");
                    exit(EXIT_FAILURE);
                }
        }
    }

    return 0;
}
