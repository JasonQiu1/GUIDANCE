#ifndef PATHUTIL_H
#define PATHUTIL_H

/*
 *  Util functions for dealing with paths.
*/

#define MAX_PATH_LEN 1024

// creates all directories in a path if they don't exist
// returns the number of directories created,
// returns -1 if a nonexistent directory cannot be created
int mkdirs(char* path, int permBits);

// appends str to file
void appendf(char* fileName, char* str);

// Returns 1 if path exists, 0 otherwise
int pathExists(char* path);

#endif
