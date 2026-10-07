CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
TARGET = clog

SRCS = src/main.c src/parser.c src/analyzer.c src/search.c src/report.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) reports/report.txt

rebuild: clean all

.PHONY: all clean rebuild