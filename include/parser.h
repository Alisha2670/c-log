#ifndef PARSER_H
#define PARSER_H

#define MAX_DATE_LEN 16
#define MAX_TIME_LEN 16
#define MAX_LEVEL_LEN 16
#define MAX_MSG_LEN 1024

#define MAX_ENTRIES 1000
#define MAX_INVALID_ENTRIES 100

typedef struct {
    char date[MAX_DATE_LEN];
    char time[MAX_TIME_LEN];
    char level[MAX_LEVEL_LEN];
    char message[MAX_MSG_LEN];
} LogEntry;

// Returns 1 if parsed and validated successfully
// 0 if invalid
int parse_line(const char *line, LogEntry *entry);

#endif