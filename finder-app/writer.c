#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open syslog
    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);

    const char *file_path = argv[1];
    const char *string_to_write = argv[2];

    // Try to open the file
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        syslog(LOG_ERR, "Error opening file %s: %s", file_path, strerror(errno));
        fprintf(stderr, "Error opening file %s: %s\n", file_path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Write the string to the file
    if (fprintf(file, "%s", string_to_write) < 0) {
        syslog(LOG_ERR, "Error writing to file %s: %s", file_path, strerror(errno));
        fprintf(stderr, "Error writing to file %s: %s\n", file_path, strerror(errno));
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Log the successful write operation
    syslog(LOG_DEBUG, "Writing %s to %s", string_to_write, file_path);

    // Close the file
    if (fclose(file) != 0) {
        syslog(LOG_ERR, "Error closing file %s: %s", file_path, strerror(errno));
        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Close syslog
    closelog();

    return EXIT_SUCCESS;
}

