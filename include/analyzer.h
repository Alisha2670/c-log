#ifndef ANALYZER_H
#define ANALYZER_H

#include "parser.h"

#define MAX_UNIQUE_ERRORS 100

typedef struct {
    int total;
    int info_count;
    int warning_count;
    int error_count;
    float error_rate;
} LogStats;

typedef struct {
    char message[MAX_MSG_LEN];
    int count;
} ErrorFrequency;

void analyze_logs(const LogEntry entries[], int count, LogStats *stats);
void print_statistics(const LogStats *stats);
void find_recurring_errors(const LogEntry entries[], int count);

#endif