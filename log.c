#define MAX_LOG_LINE_LEN 2048

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "log.h"

char logInfo[MAX_LOG_INFO_LEN];

static char gameLogPath[MAX_LOG_LINE_LEN];
static char progLogPath[MAX_LOG_LINE_LEN];
static char logLine[MAX_LOG_LINE_LEN];

// create log files and redirect stdout and stderr to program log file
// TODO: make the parent directory creation more robust, and follow
// the description in log.h
// returns -1 if a log folder can't be found
// returns -2 if the gsh log folder can't be created
// returns -3 if the log file itself can't be created or accessed
int initLog() {
    char logDir[MAX_LOG_LINE_LEN];
    FILE* fp = NULL;

    // try these directories for logs
    if (getenv("XDG_STATE_HOME")) {
        strncpy(logDir, getenv("XDG_DATA_HOME"), 1028);
    } else if (getenv("HOME")) {
        strncpy(logDir, getenv("HOME"), 1028);
        strncat(logDir, "/.local/state", 20);
    } else {
        fprintf(stderr, "Can't find a state directory.\n");
        return -1;
    }

    strncat(logDir, "/gsh", 5);
    if (mkdir(logDir, 0755) == -1 && errno != EEXIST) {
        perror("mkdir"); 
        return -2;
    }

    strncpy(progLogPath, logDir, 1028);
    strncat(progLogPath, "/program.log", 20);

    // try checking these folders for a data home
    if (getenv("XDG_DATA_HOME")) {
        strncpy(logDir, getenv("XDG_DATA_HOME"), 1028);
    } else if (getenv("HOME")) {
        strncpy(logDir, getenv("HOME"), 1028);
        strncat(logDir, "/.local/share", 20);
    } else {
        fprintf(stderr, "Can't find a data directory.\n");
        return -1;
    }

    strncat(logDir, "/gsh", 5);
    if (mkdir(logDir, 0755) == -1 && errno != EEXIST) {
        perror("mkdir"); 
        return -2;
    }

    strncpy(gameLogPath, logDir, 1028);
    strncat(gameLogPath, "/game.log", 20);

    // try creating/accessing the log files
    if ((fp = fopen(progLogPath, "a+"))) {
        fclose(fp); 
    } else {
        perror("fopen program log");
        return -3;
    }

    if ((fp = fopen(gameLogPath, "a+"))) {
        fclose(fp); 
    } else {
        perror("fopen game log");
        return -3;
    }

    return 0;
}

static void printToFile(char* fileName, char* str) {
    FILE* fp = fopen(fileName, "a");
    if (!fp) {
        perror("Failed to open log file!");
    }
    fprintf(fp, str);
    fclose(fp);
}

// append $USER to a string
// if no $USER then nothing
static char* appendUser(char* str) {
    if (getenv("USER")) strncat(str, getenv("USER"), 50);
    return str;
}

static char* rmNl(char* str) {
    for (char* pStr = str; *pStr; pStr++) {
        if (*pStr == '\n') *pStr = '\0';
    }
    return str;
}

static time_t rawtime;
void logPrint(LogType logType, char* str) {
    memset(logLine, 0, MAX_LOG_LINE_LEN);
    if (logType == INFO) {
        printToFile(gameLogPath, str);
        printToFile(gameLogPath, "\n");
    } else {
        appendUser(logLine);
        strncat(logLine, " (", 3);
        time(&rawtime);
        strncat(logLine, rmNl(asctime(localtime(&rawtime))), 100);
        strncat(logLine, ") [", 4);
        switch (logType) {
            case PROGINFO:
                strncat(logLine, "PROGINFO", 11);
                break;
            case WARNING:
                strncat(logLine, "WARNING", 10);
                break;
            case CRITICAL:
                strncat(logLine, "CRITICAL", 10);
                break;
            default:
                break;
        }
        strncat(logLine, "]: ", 4);
        strncat(logLine, str, MAX_LOG_LINE_LEN - 150);
        printToFile(progLogPath, logLine);
        printToFile(progLogPath, "\n");
    }
}
