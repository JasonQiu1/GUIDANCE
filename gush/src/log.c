#define MAX_LOG_LINE_LEN 2048

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"
#include "pathutil.h"
#include "strutil.h"

// A string to build log strings in.
char logInfo[MAX_LOG_INFO_LEN];

static char gameLogPath[MAX_LOG_LINE_LEN];
static char progLogPath[MAX_LOG_LINE_LEN];
static char logLine[MAX_LOG_LINE_LEN];

// create log files and redirect stdout and stderr to program log file
// returns -1 if a log folder can't be found
// returns -2 if the guidance log folder can't be created
// returns -3 if the log file itself can't be created or accessed
// otherwise 0
int initLog() {
    char logDir[MAX_LOG_LINE_LEN];
    FILE* fp = NULL;

    // try these directories for logs
    if (getenv("XDG_STATE_HOME")) {
        strncpy(logDir, getenv("XDG_DATA_HOME"), MAX_PATH_LEN);
    } else if (getenv("HOME")) {
        strncpy(logDir, getenv("HOME"), MAX_PATH_LEN);
        strncat(logDir, "/.local/state", 20);
    } else {
        fprintf(stderr, "Can't find a state directory.\n");
        return -1;
    }

    strncat(logDir, "/guidance", 20);
    if (mkdirs(logDir, 0755) == -1) {
        perror("mkdir"); 
        return -2;
    }

    strncpy(progLogPath, logDir, MAX_PATH_LEN);
    strncat(progLogPath, "/program.log", 20);

    // try checking these folders for a data home
    if (getenv("XDG_DATA_HOME")) {
        strncpy(logDir, getenv("XDG_DATA_HOME"), MAX_PATH_LEN);
    } else if (getenv("HOME")) {
        strncpy(logDir, getenv("HOME"), MAX_PATH_LEN);
        strncat(logDir, "/.local/share", 20);
    } else {
        fprintf(stderr, "Can't find a data directory.\n");
        return -1;
    }

    strncat(logDir, "/guidance", 20);
    if (mkdirs(logDir, 0755) == -1) {
        perror("mkdir"); 
        return -2;
    }

    strncpy(gameLogPath, logDir, MAX_PATH_LEN);
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

static time_t rawtime;
void appendLog(LogType logType, char* str) {
    memset(logLine, 0, MAX_LOG_LINE_LEN);
    if (logType == INFO) {
        appendf(gameLogPath, str);
        appendf(gameLogPath, "\n");
    } else {
        strcatuser(logLine);
        strncat(logLine, " (", 3);
        time(&rawtime);
        strncat(logLine, strrmnl(asctime(localtime(&rawtime))), 100);
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
        appendf(progLogPath, logLine);
        appendf(progLogPath, "\n");
    }
}
