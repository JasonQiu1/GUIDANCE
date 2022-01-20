#include <stdlib.h>
#include <string.h>

#include "strutil.h"

// Return number of characters until next delimited character or null term
int strtoklen(char* str, const char* delim) {
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

// replace \n characters with NUL
char* strrmnl(char* str) {
    for (char* pStr = str; *pStr; pStr++) {
        if (*pStr == '\n') *pStr = '\0';
    }
    return str;
}

// returns a new char pointer to start of non whitespace characters
char* strskipspace(char* str) {
    char* pStr = str;
    while (*pStr == ' ') pStr++;
    return pStr;
}

// append $USER to a string
// if no $USER then nothing
char* strcatuser(char* str) {
    if (getenv("USER")) strncat(str, getenv("USER"), 50);
    return str;
}
