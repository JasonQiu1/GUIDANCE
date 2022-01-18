#ifndef NCURSESUTIL_H
#define NCURSESUTIL_H

/*
 * Provides functions to initialize and cleanup ncurses windows.
*/

extern int lines, columns;

// Initialize ncurses
void initNcurses();

// Clean up ncurses
void cleanupNcurses();

#endif
