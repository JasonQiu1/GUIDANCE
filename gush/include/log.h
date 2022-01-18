#ifndef LOG_H
#define LOG_H

/*
 *  Provides functions to log game and program info.
 *  Game info gets logged to $XDG_DATA_HOME/gsh/game#/$USER/game.log or 
 *                           $HOME/.local/share/gsh/game#/$USER/game.log
 *  Program info goes to     $XDG_STATE_HOME/gsh/game#/prog.log or
 *                           $HOME/.local/state/gsh/game#/prog.log
*/

#define MAX_LOG_INFO_LEN 1024

extern char logInfo[MAX_LOG_INFO_LEN];

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
