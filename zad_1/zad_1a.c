#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    uid_t uid = getuid();
    gid_t gid = getgid();
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgrp();

    if (pgid == -1) {
        perror("Nie mozna uzyskac identyfikatora PGID!\n");
        exit(EXIT_FAILURE);
    }

    printf("Identyfikatory dla biezacego procesu:\nUID: %u\nGID: %u\nPID: %d\nPPID: %d\nPGID: %d\n", uid, gid, pid, ppid, pgid);
    return 0;
}
