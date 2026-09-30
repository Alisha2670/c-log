#ifndef SEARCH_H
#define SEARCH_H

#include "parser.h"

void filter_by_level(const LogEntry entries[], int count, const char *level);
void search_by_keyword(const LogEntry entries[], int count, const char *keyword);

#endif