#define MAX_FILE_LINE_LEN 2048

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "log.h"
#include "pathutil.h"
#include "strutil.h"

static char dataPath[MAX_PATH_LEN] = {0};
static char tempDataPath[MAX_PATH_LEN];
static FILE* fp;

// Checks for data file and initializes temporary data buffer file.
// dataPath: $XDG_DATA_HOME/guidance/data or $HOME/.local/share/guidance/data
// tempDataPath: $XDG_DATA_HOME/guidance/day or $HOME/.local/share/guidance/day
int initData() {
    char dataDir[MAX_PATH_LEN];

    // try checking these folders for a data home
    if (getenv("XDG_DATA_HOME")) {
        strncpy(dataDir, getenv("XDG_DATA_HOME"), MAX_PATH_LEN);
    } else if (getenv("HOME")) {
        strncpy(dataDir, getenv("HOME"), MAX_PATH_LEN);
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

    strncpy(dataPath, dataDir, MAX_PATH_LEN);
    strncat(dataPath, "/data", 6);
    if (!pathExists(dataPath)) {
        fprintf(stderr, "Data file not found.\n");
        return -2;
    }

    strncpy(tempDataPath, dataDir, MAX_PATH_LEN);
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

// Seeks out the line after a matching TOML section
// Returns -1 if section not found, 0 otherwise
static int fseekAfterSection(FILE* fp, char* section) {
    char dataLine[MAX_FILE_LINE_LEN];
    int sectionLen = strlen(section);
    do {
        if (!fgets(dataLine, MAX_FILE_LINE_LEN, fp)) return -1;
    } while(strncmp(dataLine + 1, section, sectionLen));
    return 0;
}

// Seeks out the line containing the given key
// Returns -1 if the key is not found, 0 otherwise
static int fseekKey(FILE* fp, char* key) {
    char dataLine[MAX_FILE_LINE_LEN];
    fpos_t pos;
    int keyLen = strlen(key);
    while (!fgetpos(fp, &pos) && fgets(dataLine, MAX_FILE_LINE_LEN, fp)) {
        if (!strncmp(dataLine + 4, key, keyLen)) {
            fsetpos(fp, &pos);
            return 0;
        }
    }
    return -1;
}

// Reads a user's data (TOML). TODO: read data file if can't find in temp
// If userName is NULL, then use the player's username.
// returns NULL if any problems occur
char* readData(char* userName, char* section, char* key) {
    char userDataPath[MAX_PATH_LEN];
    char dataLine[MAX_FILE_LINE_LEN];
    int keyLen = strlen(key);
    int valueLen;
    char* pValue;
    char* value;

    strncpy(userDataPath, getenv("HOME"), MAX_PATH_LEN);
    if (userName) {
        strncpy(userDataPath + strtoklen(userDataPath + 1, "/")+2, userName, MAX_PATH_LEN);
    }
    strncat(userDataPath, "/.local/share/guidance/day", 100);

    if (!(fp = fopen(userDataPath, "r"))) {
        perror("fopen user data");
        return NULL;
    }

    // search for section
    if (fseekAfterSection(fp, section) == -1) {
        return NULL; 
    }

    // find key value pair
    do {
        fgets(dataLine, MAX_FILE_LINE_LEN, fp);
        pValue = strskipspace(dataLine);
    } while (*dataLine && strncmp(pValue, key, keyLen));

    // extract value
    pValue = pValue + strtoklen(pValue, " ") + 1;
    valueLen = strlen(pValue) + 1; //incl NUL
    value = malloc(sizeof *value * valueLen);
    strncpy(value, pValue, valueLen);

    fclose(fp);
    return value;
}

// Writes to a temporary buffer for the day. (TOML)
// Gucron will deal with updating the actual data files.
int writeData(char* section, char* key, char* value) {
    // open file
    if (!(fp = fopen(tempDataPath, "w"))) {
        fprintf(stderr, "Cannot open temp data file!\n");
        return INT_MIN;    
    }
   
    // append new section to end of file if doesn't exist 
    if (fseekAfterSection(fp, section) == -1) {
        fprintf(fp, "[%s]\n", section);
    }

    // if key value pair exists, then update it,
    // otherwise, make it
    fseekKey(fp, key);
    fprintf(fp, "    %s %s", key, value); 

    fclose(fp);
    return 0;
}
