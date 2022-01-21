#ifndef STRUTIL_H
#define STRUTIL_H

#define MAX_TMPSTR_LEN 10240

#include <stdarg.h>

typedef struct string {
    char* str;
    int* strLen;
    int maxLen;
} string;

// For string building NOT THREAD SAFE
extern char tmpstr[MAX_TMPSTR_LEN];
extern int tmpstrLen;

int safesnprintf(string dest, char* format, ...);
int vsafesnprintf(string dest, char* format, va_list args);
int safestrncat(string dest, char* source, int n);
int safestrncpy(string dest, char* source, int n);

// safe snprintf into tmpstr. automatically keeps track of tmpstrLen
int tmpstrprintf(char* format, ...);

// safe strncat into tmpstr. automatically keeps track of tmpstrLen
int tmpstrncat(char* source, int n);

// safe strncpy to tmpstr
int tmpstrncpy(char* source, int n);

// Return number of characters until next delimited character or null term
int strtoklen(char* str, const char* delim);

// replace \n characters with NUL
char* strrmnl(char* str);

// returns a new char pointer to start of non whitespace characters
char* strskipspace(char* str);

// append $USER to a string
// if no $USER then nothing
char* strcatuser(char* str);

#endif
