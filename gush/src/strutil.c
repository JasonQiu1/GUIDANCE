#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "strutil.h"

// For string building NOT THREAD SAFE
char tmpstr[MAX_TMPSTR_LEN];
int tmpstrLen = 0;

int safesnprintf(string dest, char* format, ...) {
    va_list args;
    va_start(args, format);
    *dest.strLen = vsnprintf(dest.str, dest.maxLen, format, args);
    va_end(args);

    if (*dest.strLen > dest.maxLen) {
        return -1;
    }
    return 0;
}

int vsafesnprintf(string dest, char* format, va_list args) {
    *dest.strLen = vsnprintf(dest.str, dest.maxLen, format, args);
    if (*dest.strLen > dest.maxLen) {
        return -1;
    }
    return 0;
}

int safestrncat(string dest, char* source, int n) {
    if (*dest.strLen + n > dest.maxLen) {
        return -1;
    }

    strncat(dest.str, source, dest.maxLen - *dest.strLen - n);
    *dest.strLen += strnlen(source, n);
    return 0;
}

int safestrncpy(string dest, char* source, int n) {
    if (n > dest.maxLen) {
        return -1;
    }
    
    *dest.strLen = stpncpy(dest.str, source, n) - dest.str;
    if (*dest.strLen == n && n != dest.maxLen && dest.str[n] != '\0') {
        dest.str[n] = '\0';
    }

    return 0;
}

// safe snprintf into tmpstr. automatically keeps track of tmpstrLen
int tmpstrprintf(char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vsafesnprintf((string){ tmpstr, &tmpstrLen, MAX_TMPSTR_LEN }, format, args);
    va_end(args);
    return ret;
}

// safe strncat into tmpstr. automatically keeps track of tmpstrLen
int tmpstrncat(char* source, int n) {
    return safestrncat((string){ tmpstr, &tmpstrLen, MAX_TMPSTR_LEN }, source, n);
}

// safe strncpy to tmpstr
int tmpstrncpy(char* source, int n) {
    return safestrncpy((string){ tmpstr, &tmpstrLen, MAX_TMPSTR_LEN }, source, n);
}

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
