# C-Log: Log Analysis & Diagnostic Tool

[![Language: C](https://img.shields.io/badge/Language-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![Platform: Linux](https://img.shields.io/badge/Platform-Ubuntu%20Linux-orange.svg)](https://ubuntu.com/)
[![Build: Make](https://img.shields.io/badge/Build-Makefile-green.svg)](Makefile)
[![Memory Safety: Valgrind](https://img.shields.io/badge/Valgrind-0%20Leaks-brightgreen.svg)](#7-memory-safety--valgrind-verification)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**C-Log** is a fast, robust command-line log analysis and diagnostic tool written in pure C99 for Linux environments. It safely ingests system and application log files, parses multi-word entries, validates data integrity, computes severity statistics, isolates root-cause recurring errors, and exports comprehensive diagnostic health reports.

---

## 1. Project Proposal & Problem Statement

Production servers, cloud clusters, and microservices generate millions of lines of unstructured logs every day. When an outage occurs, finding the root cause manually using traditional text viewers is slow, inefficient, and error-prone.

**C-Log** solves this problem by providing an intelligent diagnostic pipeline that:
* Ingests any standard log file without crashing on corrupted or malformed lines.
* Computes analytical severity metrics (counts and error percentages) instantly.
* Allows engineers to filter logs by severity level, calendar date, or case-insensitive keywords.
* Identifies top recurring error messages to pinpoint the root cause of failures.
* Exports executive diagnostic summaries to disk.
* Dynamically scales to arbitrary file sizes using heap memory management with **0 memory leaks**.

---

## 2. Expected Log Format

C-Log expects log entries formatted as:
```text
YYYY-MM-DD HH:MM:SS LEVEL MESSAGE
```

### Example:
```text
2026-09-26 10:15:22 INFO Server started successfully
2026-09-26 10:18:41 WARNING High memory consumption detected
2026-09-26 10:20:15 ERROR Database connection failed
```
* **Date:** 10-character ISO format (`YYYY-MM-DD`).
* **Time:** 8-character 24-hour format (`HH:MM:SS`).
* **Level:** Strictly validated against `INFO`, `WARNING`, or `ERROR`.
* **Message:** Multi-word sentence preserved in full without word truncation.

---

## 3. Architecture & Data Flow

```
                      +-------------------+
                      |   Input .log File |
                      +---------+---------+
                                |
                                v
                      +-------------------+
                      |  fgets() Stream   | (Safe buffer bounds)
                      +---------+---------+
                                |
                                v
                      +-------------------+
                      |     Parser        | (Validates fields & log level)
                      +----+---------+----+
                           |         |
                  Valid    |         | Invalid
                           v         v
                +------------+     +-------------------+
                | Heap Array |     | Invalid Lines Log |
                | (malloc /  |     +-------------------+
                |  realloc)  |
                +-----+------+
                      |
        +-------------+-------------+-------------+
        |                           |             |
        v                           v             v
+---------------+           +---------------+   +-------------------+
|  Analyzer     |           | Search/Filter |   | Report Generator  |
|  - Stats      |           | - Level       |   | - Terminal output |
|  - Error Rate |           | - Date        |   | - Export to disk  |
|  - Recurring  |           | - Substring   |   |   (report.txt)    |
+---------------+           +---------------+   +-------------------+
```

---

## 4. Project Directory Layout

```text
c-log/
├── .gitignore          # Ignores compiled binaries and temporary artifacts
├── Makefile            # Automated build script (all, clean, rebuild)
├── README.md           # Project documentation and proposal
├── LICENSE             # Open-source MIT License
├── task.md             # 10-Day progressive milestone tracker
├── data/               # Test datasets
│   ├── sample.log      # Primary test log (10 lines)
│   ├── large_test.log  # Benchmark test log (5,000 lines)
│   ├── empty.log       # Edge-case: 0-byte file
│   └── corrupt.log     # Edge-case: 100% malformed lines
├── include/            # Header interface contracts
│   ├── parser.h        # LogEntry definition and parser prototype
│   ├── analyzer.h      # LogStats, ErrorFrequency, and analytics prototypes
│   ├── search.h        # Filtering and keyword search prototypes
│   └── report.h        # Diagnostic reporting prototypes
├── reports/            # Output directory for exported reports
│   └── report.txt      # Generated diagnostic report
└── src/                # Implementation source code
    ├── main.c          # Entry point and CLI coordinator
    ├── parser.c        # Safe line parsing and field extraction
    ├── analyzer.c      # Severity metrics and recurring error counter
    ├── search.c        # Level, date, and keyword query filters
    └── report.c        # Executive diagnostic report generator
```

---

## 5. Prerequisites & Compilation

### Prerequisites
* **OS:** Ubuntu Linux (or Windows WSL with Ubuntu)
* **Compiler:** `gcc` (with C99 support)
* **Build System:** `make`
* **Memory Profiler:** `valgrind` (optional, for leak verification)

### Building the Project
Clone the repository and run `make`:
```bash
git clone https://github.com/Alisha2670/c-log.git
cd c-log
make
```

To clean all compiled binaries:
```bash
make clean
```

---

## 6. CLI Usage & Features

### 1. Overall Summary & Statistics (Default)
```bash
./clog data/sample.log
```
**Sample Output:**
```text
========== VALIDATION SUMMARY ==========
Total Lines Processed : 10
Valid Entries Stored  : 8
Invalid Entries       : 2

Invalid Entries Found on Line(s):
  - Line 3
  - Line 5
========================================

========== LOG STATISTICS ==========
Total Entries : 8
INFO          : 3 (37.50%)
WARNING       : 1 (12.50%)
ERROR         : 4 (50.00%)

Error Rate    : 50.00%
====================================
```

### 2. High-Volume Benchmark (5,000 Lines in 11ms)
```bash
time ./clog data/large_test.log --report
```

### 3. Severity Filtering
Filter logs by severity level:
```bash
./clog data/sample.log --errors
./clog data/sample.log --warnings
./clog data/sample.log --info
```

### 4. Date Filtering
Filter logs belonging to a specific calendar date:
```bash
./clog data/sample.log --date 2026-09-27
```

### 5. Case-Insensitive Keyword Search
Search the message body for a specific term:
```bash
./clog data/sample.log --search database
```

### 6. Root Cause Analysis (Recurring Errors)
Isolate and count repeating errors to identify primary failure points:
```bash
./clog data/sample.log --recurring
```
**Sample Output:**
```text
========== RECURRING ERRORS (ROOT CAUSE) ==========
Occurrences  | Error Message
-------------+-------------------------------------
     3       | Database connection failed
     1       | Authentication failed
===================================================
```

### 7. Full Diagnostic Report & File Export
Generate an executive report and export it to `reports/report.txt`:
```bash
./clog data/sample.log --report --export
```

---

## 7. Memory Safety & Valgrind Verification

C-Log uses dynamic heap memory management (`malloc` / `realloc` / `free`) to scale dynamically without memory limits.

Run memory analysis with Valgrind on 5,000 lines:
```bash
valgrind --leak-check=full ./clog data/large_test.log
```

**Valgrind Verification Output on 5,000 Lines:**
```text
==1904== HEAP SUMMARY:
==1904==     in use at exit: 0 bytes in 0 blocks
==1904==   total heap usage: 13 allocs, 13 frees, 10,972,152 bytes allocated
==1904== 
==1904== All heap blocks were freed -- no leaks are possible
==1904== 
==1904== ERROR SUMMARY: 0 errors from 0 contexts
```

---

## 8. Key Engineering Design Decisions

1. **Defensive Input Handling:** `fgets()` is used with fixed buffer bounds (`MAX_LINE_LENGTH 1024`) instead of `gets()` or `fscanf()`, eliminating buffer overflow vulnerabilities.
2. **Dynamic Memory Scaling:** A geometric growth strategy (`capacity *= 2` via `realloc`) allows C-Log to scale from 10 entries to hundreds of thousands without stack overflow.
3. **Linear Search for Recurring Errors:** An array of structs with linear deduplication was selected over a hash table because unique error messages typically number in the dozens, achieving microsecond execution times while eliminating hashing complexity.
4. **Stream-Based Reporting:** A unified reporting engine accepts any `FILE *` stream, allowing identical formatting for terminal display (`stdout`) and file export (`fopen`).

---

## 9. Limitations & Future Enhancements

* **Future Enhancement:** Time-series error analysis to identify peak error spikes across hourly windows.
* **Future Enhancement:** JSON/CSV report export format for integration with Grafana and ELK stack.
* **Current Limitation:** Log format must strictly follow the `YYYY-MM-DD HH:MM:SS LEVEL MESSAGE` structure.

---

## 10. License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.