#ifndef DATA_H
#define DATA_H

/*
 *  Functions for reading and writing user data (TOML).
*/

// Checks for data file and initializes temporary data buffer file.
int initData();

// Reads a user's data.
char* readData(char* userName, char* section, char* key);

// Writes to a temporary buffer for the day. 
// Gucron will deal with updating the actual data files.
int writeData(char* section, char* key, char* value);

#endif
