#ifndef LOG_H
#define LOG_H

#define MAX_LOG_LINE_SIZE 1024

typedef enum LogType {
    INFO,
    PROGINFO,
    WARNING,
    CRITICAL
} LogType;

// get log file locations and redirect stdout and stderr to program log file
// returns -1 if a log folder can't be found
// returns -2 if the gsh log folder can't be created
// returns -3 if the log file itself can't be created or accessed
int initLog();

void logPrint(LogType type, char* str);

#endif
