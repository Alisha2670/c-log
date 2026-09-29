#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "analyzer.h"

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

    LogEntry entries[MAX_ENTRIES];
    int valid_count = 0;

    int invalid_lines[MAX_INVALID_ENTRIES];
    int invalid_count = 0;

    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_number = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        if (parse_line(line, &entry)) {
            if (valid_count < MAX_ENTRIES) {
                entries[valid_count] = entry;
                valid_count++;
            }
        } else {
            if (invalid_count < MAX_INVALID_ENTRIES) {
                invalid_lines[invalid_count] = line_number;
                invalid_count++;
            }
        }
    }

    fclose(file);

    // 1. Display Validation Summary
    printf("========== VALIDATION SUMMARY ==========\n");
    printf("Total Lines Processed : %d\n", line_number);
    printf("Valid Entries Stored  : %d\n", valid_count);
    printf("Invalid Entries       : %d\n", invalid_count);

    if (invalid_count > 0) {
        printf("\nInvalid Entries Found on Line(s):\n");
        for (int i = 0; i < invalid_count; i++) {
            printf("  - Line %d\n", invalid_lines[i]);
        }
    }
    printf("========================================\n");

    // 2. Compute and Display Statistics (Day 4)
    LogStats stats;
    analyze_logs(entries, valid_count, &stats);
    print_statistics(&stats);

    return EXIT_SUCCESS;
}