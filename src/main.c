#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No log file specified.\n");
        fprintf(stderr, "Usage: %s <path_to_log_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filepath = argv[1];

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    printf("--- Reading Log File: %s ---\n", filepath);
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    printf("\n--- End of File ---\n");

    fclose(file);
    return EXIT_SUCCESS;
}