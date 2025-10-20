#include <stdio.h>
#include <unistd.h>     // for fork(), pipe()
#include <sys/wait.h>   // for waitpid()
#include <string.h>     // for strlen(), strcpy()
#include <stdlib.h>     // for malloc()

int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        return 1;
    } 
    else if (pid == 0) {
        // ---- CHILD PROCESS ----
        close(fd[1]); // Close write end

        // First read how many arguments were sent
        int arg_count;
        read(fd[0], &arg_count, sizeof(int));

        // Allocate array to store arguments
        char **child_argv = malloc((arg_count + 1) * sizeof(char *));
        if (!child_argv) {
            perror("malloc failed");
            return 1;
        }

        // Read each argument string
        for (int i = 0; i < arg_count; i++) {
            int len;
            read(fd[0], &len, sizeof(int)); // length of string
            child_argv[i] = malloc(len + 1);
            read(fd[0], child_argv[i], len);
            child_argv[i][len] = '\0';
        }
        child_argv[arg_count] = NULL; // end of argv[]

        close(fd[0]);

        // ✅ Child received argv[] as separate elements
        printf("Child received arguments:\n");
        for (int i = 0; i < arg_count; i++) {
            printf("  argv[%d] = %s\n", i + 1, child_argv[i]);
        }

        // Example usage
        if (arg_count >= 1 && strcmp(child_argv[0], "config") == 0) {
            printf("Ok");
        } else {
            printf("NO");
        }

        // Free memory
        for (int i = 0; i < arg_count; i++) {
            free(child_argv[i]);
        }
        free(child_argv);
    } 
    else {
        // ---- PARENT PROCESS ----
        close(fd[0]); // Close read end

        // Send total number of arguments (excluding program name)
        int arg_count = argc - 1;
        write(fd[1], &arg_count, sizeof(int));

        // Send each argument separately
        for (int i = 1; i < argc; i++) {
            int len = strlen(argv[i]);
            write(fd[1], &len, sizeof(int));
            write(fd[1], argv[i], len);
        }

        close(fd[1]);

        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
