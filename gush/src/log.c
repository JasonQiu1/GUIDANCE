#define MAX_LOG_LINE_LEN 2048

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"
#include "pathutil.h"
#include "strutil.h" //tmpstr, tmpstrLen

static char gameLogPath[MAX_LOG_LINE_LEN];
static char progLogPath[MAX_LOG_LINE_LEN];

// create log files and redirect stdout and stderr to program log file
// returns -1 if a log folder can't be found
// returns -2 if the guidance log folder can't be created
// returns -3 if the log file itself can't be created or accessed
// otherwise 0
int initLog() {
    FILE* fp = NULL;

    // try these directories for logs
    if (getenv("XDG_STATE_HOME")) {
        tmpstrncpy(getenv("XDG_DATA_HOME"), MAX_PATH_LEN);
    } else if (getenv("HOME")) {
        tmpstrncpy(getenv("HOME"), MAX_PATH_LEN);
        tmpstrncat("/.local/state", 20);
    } else {
        fprintf(stderr, "Can't find a state directory.\n");
        return -1;
    }

    tmpstrncat("/guidance", 20);
    if (mkdirs(tmpstr, 0755) == -1) {
        perror("mkdir"); 
        return -2;
    }

    strncpy(progLogPath, tmpstr, MAX_PATH_LEN);
    strncat(progLogPath, "/program.log", 20);

    // try checking these folders for a data home
    if (getenv("XDG_DATA_HOME")) {
        tmpstrncpy(getenv("XDG_DATA_HOME"), MAX_PATH_LEN);
    } else if (getenv("HOME")) {
        tmpstrncpy(getenv("HOME"), MAX_PATH_LEN);
        tmpstrncat("/.local/share", 20);
    } else {
        fprintf(stderr, "Can't find a data directory.\n");
        return -1;
    }

    tmpstrncat("/guidance", 20);
    if (mkdirs(tmpstr, 0755) == -1) {
        perror("mkdir"); 
        return -2;
    }

    strncpy(gameLogPath, tmpstr, MAX_PATH_LEN);
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
void appendLog(LogType logType, char* contentStr) {
    if (logType == INFO) {
        appendf(gameLogPath, contentStr);
        appendf(gameLogPath, "\n");
    } else {
        char logLine[MAX_TMPSTR_LEN];
        int len = 0;
        char* userStr = getenv("USER");
        time(&rawtime);
        char* timeStr = strrmnl(asctime(localtime(&rawtime)));
        char logTypeStr[20];
        switch (logType) {
            case PROGINFO:
                strcpy(logTypeStr, "PROGINFO");
                break;
            case WARNING:
                strcpy(logTypeStr, "WARNING");
                break;
            case CRITICAL:
                strcpy(logTypeStr, "CRITICAL");
                break;
            default:
                strcpy(logTypeStr, "");
                break;
        }
        safesnprintf((string){logLine, &len, MAX_TMPSTR_LEN}, "%s (%s) [%s]: %s\n", userStr, timeStr, logTypeStr, contentStr);
        appendf(progLogPath, logLine);
    }
}
