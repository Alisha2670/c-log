#include <stdio.h>
#include <string.h>
#include "parser.h"

int parse_line(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) {
        return 0;
    }

    int fields = sscanf(line, "%15s %15s %15s %1023[^\r\n]",
                        entry->date,
                        entry->time,
                        entry->level,
                        entry->message);

    if (fields < 4) {
        return 0;
    }

    if (strcmp(entry->level, "INFO") != 0 &&
        strcmp(entry->level, "WARNING") != 0 &&
        strcmp(entry->level, "ERROR") != 0) {
        return 0;
    }

    return 1;
}