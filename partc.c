#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s FILENAME\n", argv[0]);
        return 1;
    }

    char *input_filename = argv[1];

    // Create a pipe for inter-process communication
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error creating pipe");
        return 1;
    }

    int pipe_read_fd = pipefd[0];
    int pipe_write_fd = pipefd[1];

    // Create child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error forking");
        return 1;
    }

    if (pid > 0) {
        // Parent process - executes parta
        
        // Close the read end of the pipe (parent only writes)
        close(pipe_read_fd);

        // Redirect STDOUT to the pipe's write end
        dup2(pipe_write_fd, STDOUT_FILENO);

        // Close the original write fd after duplication
        close(pipe_write_fd);

        // Execute parta with the input filename
        char *parta_args[] = {"./parta", input_filename, NULL};
        execv("./parta", parta_args);

        // If execv returns, it failed
        perror("Error executing parta");
        return 1;
    } else {
        // Child process - executes sort
        
        // Close the write end of the pipe (child only reads)
        close(pipe_write_fd);

        // Redirect STDIN to the pipe's read end
        dup2(pipe_read_fd, STDIN_FILENO);

        // Close the original read fd after duplication
        close(pipe_read_fd);

        // Execute sort with arguments to sort by second column numerically
        // -t, = use comma as delimiter
        // -k2 = sort by second column
        // -n = numeric sort
        char *sort_args[] = {"sort", "-t,", "-k2", "-n", NULL};
        execvp("sort", sort_args);

        // If execvp returns, it failed
        perror("Error executing sort");
        return 1;
    }

    return 0;
}