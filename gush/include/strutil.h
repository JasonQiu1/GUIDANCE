#ifndef STRUTIL_H
#define STRUTIL_H

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
