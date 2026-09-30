#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "search.h"
#include "parser.h"

void filter_by_level(const LogEntry entries[], int count, const char *level) {
    int matches = 0;
    printf("\n========== LOGS FILTERED BY LEVEL: %s ==========\n", level);

    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("[%s %s] %-7s %s\n",
                   entries[i].date, entries[i].time, entries[i].level, entries[i].message);
            matches++;
        }
    }

    printf("Total Matches: %d\n", matches);
    printf("================================================\n");
}

void search_by_keyword(const LogEntry entries[], int count, const char *keyword) {
    int matches = 0;
    printf("\n========== SEARCH RESULTS FOR: \"%s\" ==========\n", keyword);

    for (int i = 0; i < count; i++) {
        if (strcasestr(entries[i].message, keyword) != NULL) {
            printf("[%s %s] %-7s %s\n",
                   entries[i].date, entries[i].time, entries[i].level, entries[i].message);
            matches++;
        }
    }

    printf("Total Matches: %d\n", matches);
    printf("================================================\n");
}