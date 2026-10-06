#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "analyzer.h"
#include "search.h"
#include "report.h"

#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 10

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No log file specified.\n");
        fprintf(stderr, "Usage: %s <path_to_log_file> [options]\n", argv[0]);
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  --errors              Show only ERROR logs\n");
        fprintf(stderr, "  --warnings            Show only WARNING logs\n");
        fprintf(stderr, "  --info                Show only INFO logs\n");
        fprintf(stderr, "  --date <YYYY-MM-DD>   Show logs for a specific date\n");
        fprintf(stderr, "  --search <keyword>    Search messages by keyword\n");
        fprintf(stderr, "  --recurring           Show top recurring error messages\n");
        fprintf(stderr, "  --report [--export]   Generate full diagnostic report (optional export)\n");
        return EXIT_FAILURE;
    }

    const char *filepath = argv[1];

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int capacity = INITIAL_CAPACITY;
    LogEntry *entries = malloc(capacity * sizeof(LogEntry));
    if (entries == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return EXIT_FAILURE;
    }
    int valid_count = 0;

    int invalid_lines[MAX_INVALID_ENTRIES];
    int invalid_count = 0;

    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_number = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        if (parse_line(line, &entry)) {
            if (valid_count >= capacity) {
                capacity *= 2;
                LogEntry *temp = realloc(entries, capacity * sizeof(LogEntry));
                if (temp == NULL) {
                    perror("Memory reallocation failed");
                    free(entries);
                    fclose(file);
                    return EXIT_FAILURE;
                }
                entries = temp;
            }
            entries[valid_count] = entry;
            valid_count++;
        } else {
            if (invalid_count < MAX_INVALID_ENTRIES) {
                invalid_lines[invalid_count] = line_number;
                invalid_count++;
            }
        }
    }

    fclose(file);

    int exit_code = EXIT_SUCCESS;

    if (argc > 2) {
        const char *flag = argv[2];

        if (strcmp(flag, "--errors") == 0) {
            filter_by_level(entries, valid_count, "ERROR");
        } else if (strcmp(flag, "--warnings") == 0) {
            filter_by_level(entries, valid_count, "WARNING");
        } else if (strcmp(flag, "--info") == 0) {
            filter_by_level(entries, valid_count, "INFO");
        } else if (strcmp(flag, "--date") == 0) {
            if (argc < 4) {
                fprintf(stderr, "Error: --date option requires a date (YYYY-MM-DD).\n");
                fprintf(stderr, "Usage: %s %s --date <YYYY-MM-DD>\n", argv[0], filepath);
                exit_code = EXIT_FAILURE;
            } else {
                filter_by_date(entries, valid_count, argv[3]);
            }
        } else if (strcmp(flag, "--search") == 0) {
            if (argc < 4) {
                fprintf(stderr, "Error: --search option requires a keyword.\n");
                fprintf(stderr, "Usage: %s %s --search <keyword>\n", argv[0], filepath);
                exit_code = EXIT_FAILURE;
            } else {
                search_by_keyword(entries, valid_count, argv[3]);
            }
        } else if (strcmp(flag, "--recurring") == 0) {
            find_recurring_errors(entries, valid_count);
        } else if (strcmp(flag, "--report") == 0) {
            int export_to_file = 0;
            if (argc > 3 && strcmp(argv[3], "--export") == 0) {
                export_to_file = 1;
            }
            generate_report(filepath, line_number, entries, valid_count, invalid_lines, invalid_count, export_to_file);
        } else {
            fprintf(stderr, "Error: Unknown option '%s'\n", flag);
            fprintf(stderr, "Run without options to view summary statistics.\n");
            exit_code = EXIT_FAILURE;
        }
    } else {
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

        LogStats stats;
        analyze_logs(entries, valid_count, &stats);
        print_statistics(&stats);
    }

    free(entries);
    return exit_code;
}