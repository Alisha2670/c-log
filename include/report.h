#ifndef REPORT_H
#define REPORT_H

#include "parser.h"
#include "analyzer.h"

void generate_report(const char *filepath, int total_lines,
                     const LogEntry entries[], int valid_count,
                     const int invalid_lines[], int invalid_count,
                     int export_to_file);

#endif