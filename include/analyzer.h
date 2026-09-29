#ifndef ANALYZER_H
#define ANALYZER_H

#include "parser.h"

typedef struct {
    int total;
    int info_count;
    int warning_count;
    int error_count;
    float error_rate;
} LogStats;

void analyze_logs(const LogEntry entries[], int count, LogStats *stats);
void print_statistics(const LogStats *stats);

#endif