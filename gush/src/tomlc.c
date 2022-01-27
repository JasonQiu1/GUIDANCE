/*
 *  Simple TOML parser. 
 *
 *  Limited feature set:
 *  - comments and empty lines
 *  - only single quoted string literal values
 *  - only top-level table headers
*/

#define MAXFILELINE 2048

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tomlc.h"

// Contains a line from the file being read.
static char line[MAXFILELINE];

// Return number of characters until next delimited character or NUL.
static int strtoklen(char* str, const char* delim) {
    int i, len = 0;
    for (char* pStr = str; *pStr; pStr++, len++) {
        for (i = 0; delim[i]; i++) {
            if (*pStr == delim[i]) {
                return len;
            }
        }
    }
    return len;
}

// Advances the file pointer to the line after a matching table header.
// Returns 1 if table found, 0 otherwise.
static int fseekAfterHeader(FILE* fp, char* table) {
    int tableLen = strlen(table);
    do {
        if (!fgets(line, MAXFILELINE, fp)) return 0;
    } while(strncmp(line + 1, table, tableLen));
    return 1;
}

// Advances the file pointer to the line containing the given key or
//  until next header or EOF
// Returns 1 if the key is found, 0 otherwise.
static int fseekKey(FILE* fp, char* key) {
    int keyLen = strlen(key);
    fpos_t pos;
    while (!fgetpos(fp, &pos) && fgets(line, MAXFILELINE, fp)) {
        if (!strncmp(line, key, keyLen) || line[0] == '[') {
            fsetpos(fp, &pos);
            return 1;
        }
    }
    return 0;
}

// Advances the file pointer to the next key under the same header.
// Returns 1 if a key is found, 0 if header or EOF is found.
static int fseekNextKey(FILE* fp) {
    fpos_t pos;
    do {
        if (fgetpos(fp, &pos) || !fgets(line, MAXFILELINE, fp) || line[0] == '[') {
            return 0;
        }
    } while (line[0] != '#' && line[0] != '\0');
    fsetpos(fp, &pos);
    return 1;
}

// Assumes valueStr is 'value string' including the quotes
static char* extractStringValue(char* valueStr) {
    return strndup(valueStr + 1, strlen(valueStr) - 3);
}

// Returns the value of the key in the given file.
// Returns NULL if the key cannot be found.
char* readValueTOML(char* filename, char* key) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("tomlc.c:readValueTOML:fopen");
        return NULL;
    }

    int keyLen = strlen(key);
    int nextTokenLen = strtoklen(key, ".");
    char* nextToken = strndup(key, nextTokenLen);
    int ret;

    // skip to table header containing the key
    // NOTE: doesn't really need to be recursive, since only supporting
    //       top-level table headers
    while (keyLen != nextTokenLen) {
        keyLen -= nextTokenLen + 1;
        key += nextTokenLen + 1;
        
        ret = fseekAfterHeader(fp, nextToken);
        free(nextToken);
        if (!ret) {
            fclose(fp);
            return NULL;
        }

        nextTokenLen = strtoklen(key, ".");
        nextToken = strndup(key, nextTokenLen);
    } 

    // find key under table
    ret = fseekKey(fp, nextToken);
    free(nextToken);
    if (!ret) {
        fclose(fp);
        return NULL;
    }
    
    // extract value from key value pair
    fgets(line, MAXFILELINE, fp);
    fclose(fp);

    nextToken = line + keyLen + 3; // skip key and ' = '
    nextToken = extractStringValue(nextToken);

    return nextToken;
}

// Writes to dest source bytes from start to end.
// Returns number of bytes written.
static int filencpy(FILE* dest, FILE* source, long len) {
    int numBytes;
    char* buffer = malloc(len * sizeof *buffer + 1);

    fread(buffer, len, 1, source);
    numBytes = fwrite(buffer, len, 1, dest);

    free(buffer);
    return numBytes;
}

// Writes single-quoted string literal value to the key in the file.
// Returns 1 if successfully wrote, 0 otherwise.
int writeValueTOML(char* filename, char* key, char* value) {
    FILE* fpOriginal;
    char tempName[L_tmpnam + 1];
    FILE* fpTemp;
    int keyLen, nextTokenLen, keyFound;
    char* nextToken;
    long endPos, pos;
    char* bakFilename;

    // Open original data file for reading.
    fpOriginal = fopen(filename, "r");
    if (!fpOriginal) {
        perror("tomlc.c:writeValueTOML:fopen: original data file");
        return 0;
    }
    fseek(fpOriginal, 0, SEEK_END);
    endPos = ftell(fpOriginal);
    fseek(fpOriginal, 0, SEEK_SET);

    // Create new temp file for writing.
    if (!tmpnam(tempName)) {
        fclose(fpOriginal);
        perror("tomlc.c:writeValueTOML:tmpnam");
        return 0;
    }
    fpTemp = fopen(tempName, "w+");

    // Find insert position.
    keyLen = strlen(key);
    nextTokenLen = strtoklen(key, ".");
    nextToken = strndup(key, nextTokenLen);
    // Find table header if query contains one, or append if not found in file
    if (keyLen != nextTokenLen) {
        keyLen -= nextTokenLen + 1;
        key += nextTokenLen + 1;
        
        // append new table header to end of temp file if not found
        if (!fseekAfterHeader(fpOriginal, nextToken)) {
            pos = ftell(fpOriginal);        
            fseek(fpOriginal, 0, SEEK_SET);
            filencpy(fpTemp, fpOriginal, pos);
            fprintf(fpTemp, "[%s]\n", nextToken);
        }
        free(nextToken);
    }

    // write key value pair to either
    // - end of section if not found or
    // - at the old key value pair, overwriting it
    keyFound = fseekKey(fpOriginal, key);
    pos = ftell(fpOriginal);        
    fseek(fpOriginal, ftell(fpTemp), SEEK_SET);
    filencpy(fpTemp, fpOriginal, pos - ftell(fpTemp));
    fprintf(fpTemp, "%s = \'%s\'\n", key, value);

    // Write rest of original file
    if (keyFound) {
        fgets(line, MAXFILELINE, fpOriginal);
    }
    filencpy(fpTemp, fpOriginal, endPos - ftell(fpOriginal));

    fclose(fpTemp);
    fclose(fpOriginal);

    // append '.bak' to original file name
    // rename temp file to original file name
    bakFilename = strcat(strdup(filename), ".bak");
    rename(filename, bakFilename);
    free(bakFilename);
    rename(tempName, filename);

    return 1;
}

// Returns all of the keys in a table in the file. 
// Returns NULL if the table header cannot be found.
strVector* readKeysTOML(char* filename, char* header) {
    strVector* keys = malloc(sizeof *keys);
    keys->max = 1;
    keys->len = 0;
    keys->strs = malloc(keys->max * sizeof *keys->strs);

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("tomlc.c:readKeysTOML:fopen");
        return NULL;
    }

    if (!fseekAfterHeader(fp, header)) {
        fclose(fp);
        return 0;
    }

    while (fseekNextKey(fp)) {
        fgets(line, MAXFILELINE, fp);
        line[strtoklen(line, "=") - 1] = '\0';
        if (keys->len >= keys->max) {
            keys->max *= 2;
            if (!(keys = realloc(keys, keys->max))) {
                fclose(fp);
                perror("tomlc.c:readKeysTOML:realloc");
                return NULL;
            }
        }
        sscanf(line, "%ms", &keys->strs[keys->len++]);
    }

    fclose(fp);
    return keys;
}
