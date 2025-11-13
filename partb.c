#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    // Check for correct number of arguments (input file and output file)
    if (argc != 3) {
        fprintf(stderr, "USAGE: partb FILEIN FILEOUT\n");
        return 1;
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];

    // Open the output file with write, create, and truncate options, with 0770 permissions
    int fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0770);
    if (fd == -1) {
        perror("Error opening output file");
        return 1;
    }

    // Redirect STDOUT to the output file using dup2
    dup2(fd, STDOUT_FILENO);

    // Print CSV header and flush to ensure it's written before exec
    printf("Category,Count\n");
    fflush(stdout);

    // Prepare arguments for parta execution
    char *parta_args[] = {"./parta", input_filename, NULL};

    // Execute parta - all its output will go to the redirected file
    int eret = execv("./parta", parta_args);

    // If execv returns, it means it failed
    perror("Error executing parta");
    return 1;
}