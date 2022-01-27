#ifndef TOMLC_H
#define TOMLC_H

/*
 *  Simple TOML parser. 
 *
 *  Limited feature set:
 *  - comments and empty lines
 *  - only single quoted string literal values
 *  - only top-level table headers
*/

typedef struct strVector {
    int len;
    char** strs;
    int max;
} strVector;

// Returns the value of the key in the given file.
// Returns NULL if the key cannot be found.
char* readValueTOML(char* filename, char* key);

// Writes single-quoted string literal value to the key in the file.
// Returns 1 if successfully wrote, 0 otherwise.
int writeValueTOML(char* filename, char* key, char* value);

// Returns all of the keys in a table in the file. 
// Returns NULL if the table header cannot be found.
strVector* readKeysTOML(char* filename, char* header);

#endif
