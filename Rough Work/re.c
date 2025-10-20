#include <stdio.h>
#include <unistd.h>     // for fork(), pipe()
#include <sys/wait.h>   // for waitpid()
#include <string.h>     // for strlen()
#include "Config/MakeConfig.h"
#include "Errors/errors.h"

int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid;

    // Create pipe
    if (pipe(fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        forkCreationProblem();
        return 1;
    } 
    else if (pid == 0) {
        // ---- CHILD PROCESS ----
        close(fd[1]); // Close write end (child will read)

        char buffer[256];
        int n = read(fd[0], buffer, sizeof(buffer) - 1);
        if (n < 0) {
            perror("Read error in child");
            return 1;
        }
        buffer[n] = '\0'; // Null-terminate string
        close(fd[0]);

        // Print what the child received
        printf("Child received command: %s\n", buffer);

        // Simulated logic:
        if (strcmp(buffer, "config") == 0) {
            SecondAgrumentChecking(argv);
        } else {
            invalidCommand();
        }
    } 
    else {
        // ---- PARENT PROCESS ----
        close(fd[0]); // Close read end (parent will write)

        if (argc > 1) {
            // Combine all argv[] (except argv[0]) into one string
            char command[256] = "";
            for (int i = 1; i < argc; i++) {
                strcat(command, argv[i]);
                if (i < argc - 1) strcat(command, " ");
            }

            // Write command to pipe
            write(fd[1], command, strlen(command));
        } else {
            invalidCommand();
        }

        close(fd[1]); // Done writing

        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
