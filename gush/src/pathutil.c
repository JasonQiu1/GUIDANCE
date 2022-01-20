#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "pathutil.h"
#include "strutil.h"

// creates all directories in a path if they don't exist
// returns the number of directories created,
// returns -1 if a nonexistent directory cannot be created
int mkdirs(char* path, int permBits) {
    char* pathSection;
    int tokLen, offset = 1, dirsCreated = 0;
    // for each directory in the path
    while ((tokLen = strtoklen(path + offset, "/"))) {
        pathSection = strndup(path, offset + tokLen);
        // try making the directory
        if (mkdir(pathSection, permBits) == -1 && errno != EEXIST) {
            return -1;
        }
        free(pathSection);
        dirsCreated++;

        // break if at end of string
        if (offset + tokLen + 1 == '\0') {
            break;
        }
        offset += tokLen + 2; // shift offset to start of next token
    }

    return dirsCreated;
}

void appendf(char* fileName, char* str) {
    FILE* fp = fopen(fileName, "a");
    if (!fp) {
        perror("Failed to open file for appending!");
    }
    fprintf(fp, str);
    fclose(fp);
}

// Returns 1 if path exists, 0 otherwise
int pathExists(char* path) {
    struct stat buffer;
    return (stat (path, &buffer) == 0);
}
