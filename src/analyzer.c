#include <stdio.h>
#include <string.h>
#include "analyzer.h"

void analyze_logs(const LogEntry entries[], int count, LogStats *stats) {
    if (stats == NULL) return;

    stats->total = count;
    stats->info_count = 0;
    stats->warning_count = 0;
    stats->error_count = 0;
    stats->error_rate = 0.0f;

    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            stats->info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            stats->warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            stats->error_count++;
        }
    }

    if (stats->total > 0) {
        stats->error_rate = ((float)stats->error_count / stats->total) * 100.0f;
    }
}

void print_statistics(const LogStats *stats) {
    if (stats == NULL) return;

    printf("\n========== LOG STATISTICS ==========\n");
    printf("Total Entries : %d\n", stats->total);
    if (stats->total > 0) {
        printf("INFO          : %d (%.2f%%)\n", stats->info_count,
               ((float)stats->info_count / stats->total) * 100.0f);
        printf("WARNING       : %d (%.2f%%)\n", stats->warning_count,
               ((float)stats->warning_count / stats->total) * 100.0f);
        printf("ERROR         : %d (%.2f%%)\n", stats->error_count,
               ((float)stats->error_count / stats->total) * 100.0f);
    } else {
        printf("INFO          : 0 (0.00%%)\n");
        printf("WARNING       : 0 (0.00%%)\n");
        printf("ERROR         : 0 (0.00%%)\n");
    }
    printf("\nError Rate    : %.2f%%\n", stats->error_rate);
    printf("====================================\n");
}

void find_recurring_errors(const LogEntry entries[], int count) {
    ErrorFrequency freqs[MAX_UNIQUE_ERRORS];
    int unique_count = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            int found_index = -1;
            for (int j = 0; j < unique_count; j++) {
                if (strcmp(freqs[j].message, entries[i].message) == 0) {
                    found_index = j;
                    break;
                }
            }

            if (found_index != -1) {
                freqs[found_index].count++;
            } else if (unique_count < MAX_UNIQUE_ERRORS) {
                strncpy(freqs[unique_count].message, entries[i].message, MAX_MSG_LEN - 1);
                freqs[unique_count].message[MAX_MSG_LEN - 1] = '\0';
                freqs[unique_count].count = 1;
                unique_count++;
            }
        }
    }

    printf("\n========== RECURRING ERRORS (ROOT CAUSE) ==========\n");
    if (unique_count == 0) {
        printf("No errors found in the log entries!\n");
    } else {
        printf("%-12s | %s\n", "Occurrences", "Error Message");
        printf("-------------+-------------------------------------\n");
        for (int i = 0; i < unique_count; i++) {
            printf("     %-7d | %s\n", freqs[i].count, freqs[i].message);
        }
    }
    printf("===================================================\n");
}