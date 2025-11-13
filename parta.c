#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Declare the buffer to use. Do NOT change!
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];
    
    // Check for correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s FILENAME\n", argv[0]);
        return 1;
    }

    // Open the file using open() system call
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Initialize counters for each category
    int upper = 0;
    int lower = 0;
    int number = 0;
    int space = 0;
    int others = 0;

    // Read file in BUFSIZE chunks using read() system call
    int bytes_read;
    while ((bytes_read = read(fd, buffer, BUFSIZE)) > 0) {
        // Process each byte in the buffer
        for (int i = 0; i < bytes_read; i++) {
            if (isupper(buffer[i])) {
                upper++;
            } else if (islower(buffer[i])) {
                lower++;
            } else if (isdigit(buffer[i])) {
                number++;
            } else if (isspace(buffer[i])) {
                space++;
            } else {
                others++;
            }
        }
    }

    // Close the file descriptor
    close(fd);

    // Print results in required format
    printf("Upper,%d\n", upper);
    printf("Lower,%d\n", lower);
    printf("Number,%d\n", number);
    printf("Space,%d\n", space);
    printf("Other,%d\n", others);

    return 0;
}