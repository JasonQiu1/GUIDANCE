#define MAXPATH 2048

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "log.h"
#include "pathutil.h"
#include "strutil.h"
#include "tomlc.h"

static char dataPath[MAXPATH] = {0};
static char tempDataPath[MAXPATH];

// Checks for data file and initializes temporary data buffer file.
// dataPath: $XDG_DATA_HOME/guidance/data or $HOME/.local/share/guidance/data
// tempDataPath: $XDG_DATA_HOME/guidance/day or $HOME/.local/share/guidance/day
int initData() {
    char dataDir[MAXPATH];
    FILE* fp;

    // try checking these folders for a data home
    if (getenv("XDG_DATA_HOME")) {
        strncpy(dataDir, getenv("XDG_DATA_HOME"), MAXPATH);
    } else if (getenv("HOME")) {
        strncpy(dataDir, getenv("HOME"), MAXPATH);
        strncat(dataDir, "/.local/share", 20);
    } else {
        fprintf(stderr, "Can't find a data directory.\n");
        return -1;
    }

    strncat(dataDir, "/guidance", 20);
    if (mkdirs(dataDir, 0755) == -1) {
        perror("mkdir"); 
        return -2;
    }

    strncpy(dataPath, dataDir, MAXPATH);
    strncat(dataPath, "/data", 6);
    if (!pathExists(dataPath)) {
        fprintf(stderr, "Data file not found.\n");
        return -2;
    }

    strncpy(tempDataPath, dataDir, MAXPATH);
    strncat(tempDataPath, "/day", 5);

    // try creating/accessing the temp data buffer
    if ((fp = fopen(tempDataPath, "a+"))) {
        fclose(fp); 
    } else {
        perror("fopen temp data");
        return -3;
    }

    return 0; 
}

// Reads a user's data (TOML).
// Reads in this order if not found: tempDataFile -> dataFile -> gameDataFile
// If userName is NULL, then use the player's username.
// returns NULL if any problems occur
char* readData(char* userName, char* header, char* key) {
    char* userHomePath;
    char* query;
    char* value;

    tmpstrprintf("%s.%s", header, key);
    query = strdup(tmpstr);

    if (!userName) {
        tmpstrncpy(getenv("HOME"), MAXPATH);
    } else {
        tmpstrncpy(getenv("GAMEHOME"), MAXPATH);
        tmpstrncat(userName, MAXPATH);
    }
    userHomePath = strdup(tmpstr);

    tmpstrncat("/.local/share/guidance/day", MAXPATH);
    if (!(value = readValueTOML(tmpstr, query))) {
        tmpstrncpy(userHomePath, MAXPATH);
        tmpstrncat("/.local/share/guidance/data", MAXPATH);
        if (!(value = readValueTOML(tmpstr, query))) {
            if (!getenv("GAMEHOME")) {
                free(query);
                free(userHomePath);
                return NULL;
            }
            tmpstrncpy(getenv("GAMEHOME"), MAXPATH);
            tmpstrncat("/gamedata", MAXPATH);
            if (!(value = readValueTOML(tmpstr, query))) {
                free(query);
                free(userHomePath);
                return NULL;
            }
        }
    }

    free(query);
    free(userHomePath);
    return value;
}

// Writes to a temporary buffer for the day. (TOML)
// Gucron will deal with updating the actual data files.
// Returns 1 if successful, 0 otherwise.
int writeData(char* header, char* key, char* value) {
    tmpstrprintf("%s.%s", header, key);
    if (!writeValueTOML(tempDataPath, tmpstr, value)) {
        return 0;    
    }

    return 1;
}
