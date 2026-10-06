#include <stdio.h>
#include <string.h>
#include "report.h"
#include "analyzer.h"

static void write_report_content(FILE *stream, const char *filepath, int total_lines,
                                const LogEntry entries[], int valid_count,
                                const int invalid_lines[], int invalid_count) {
    LogStats stats;
    analyze_logs(entries, valid_count, &stats);

    fprintf(stream, "============================================================\n");
    fprintf(stream, "              C-LOG SYSTEM DIAGNOSTIC REPORT                \n");
    fprintf(stream, "============================================================\n");
    fprintf(stream, "Analyzed File   : %s\n", filepath);
    fprintf(stream, "Total Lines     : %d\n", total_lines);

    fprintf(stream, "\n------------------------------------------------------------\n");
    fprintf(stream, "1. INTEGRITY & VALIDATION SUMMARY\n");
    fprintf(stream, "------------------------------------------------------------\n");
    fprintf(stream, "Valid Entries Stored  : %d\n", valid_count);
    fprintf(stream, "Corrupted / Invalid   : %d\n", invalid_count);

    if (invalid_count > 0) {
        fprintf(stream, "\n* Corrupted Line Numbers:\n");
        for (int i = 0; i < invalid_count; i++) {
            fprintf(stream, "  - Line %d\n", invalid_lines[i]);
        }
    }

    fprintf(stream, "\n------------------------------------------------------------\n");
    fprintf(stream, "2. LOG SEVERITY STATISTICS\n");
    fprintf(stream, "------------------------------------------------------------\n");
    if (stats.total > 0) {
        fprintf(stream, "INFO    : %d (%.2f%%)\n", stats.info_count,
                ((float)stats.info_count / stats.total) * 100.0f);
        fprintf(stream, "WARNING : %d (%.2f%%)\n", stats.warning_count,
                ((float)stats.warning_count / stats.total) * 100.0f);
        fprintf(stream, "ERROR   : %d (%.2f%%)\n", stats.error_count,
                ((float)stats.error_count / stats.total) * 100.0f);
    }
    fprintf(stream, "\nOverall Error Rate : %.2f%%\n", stats.error_rate);

    fprintf(stream, "\n------------------------------------------------------------\n");
    fprintf(stream, "3. TOP RECURRING ERRORS (ROOT CAUSE)\n");
    fprintf(stream, "------------------------------------------------------------\n");

    ErrorFrequency freqs[MAX_UNIQUE_ERRORS];
    int unique_count = 0;
    for (int i = 0; i < valid_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            int found = -1;
            for (int j = 0; j < unique_count; j++) {
                if (strcmp(freqs[j].message, entries[i].message) == 0) {
                    found = j;
                    break;
                }
            }
            if (found != -1) {
                freqs[found].count++;
            } else if (unique_count < MAX_UNIQUE_ERRORS) {
                strncpy(freqs[unique_count].message, entries[i].message, MAX_MSG_LEN - 1);
                freqs[unique_count].message[MAX_MSG_LEN - 1] = '\0';
                freqs[unique_count].count = 1;
                unique_count++;
            }
        }
    }

    if (unique_count == 0) {
        fprintf(stream, "No errors detected.\n");
    } else {
        fprintf(stream, "%-12s | %s\n", "Occurrences", "Error Message");
        fprintf(stream, "-------------+----------------------------------------------\n");
        for (int i = 0; i < unique_count; i++) {
            fprintf(stream, "     %-7d | %s\n", freqs[i].count, freqs[i].message);
        }
    }

    fprintf(stream, "\n------------------------------------------------------------\n");
    fprintf(stream, "4. SYSTEM HEALTH VERDICT\n");
    fprintf(stream, "------------------------------------------------------------\n");
    if (stats.error_rate > 20.0f) {
        fprintf(stream, "Verdict: [ CRITICAL ATTENTION REQUIRED ]\n");
        fprintf(stream, "Reason : Error rate exceeds 20%% threshold.\n");
    } else if (stats.error_rate > 5.0f) {
        fprintf(stream, "Verdict: [ WARNING - ELEVATED ERRORS ]\n");
        fprintf(stream, "Reason : Error rate exceeds 5%% threshold.\n");
    } else {
        fprintf(stream, "Verdict: [ STABLE / HEALTHY ]\n");
        fprintf(stream, "Reason : Error rate is within normal operating limits.\n");
    }
    fprintf(stream, "============================================================\n");
}

void generate_report(const char *filepath, int total_lines,
                     const LogEntry entries[], int valid_count,
                     const int invalid_lines[], int invalid_count,
                     int export_to_file) {
    
    write_report_content(stdout, filepath, total_lines, entries, valid_count, invalid_lines, invalid_count);

    if (export_to_file) {
        FILE *file = fopen("reports/report.txt", "w");
        if (file == NULL) {
            perror("Failed to export report to reports/report.txt");
            return;
        }
        write_report_content(file, filepath, total_lines, entries, valid_count, invalid_lines, invalid_count);
        fclose(file);
        printf("\n[SUCCESS] Diagnostic report exported to: reports/report.txt\n");
    }
}