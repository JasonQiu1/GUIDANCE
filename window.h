#ifndef WINDOW_H
#define WINDOW_H

/*
 *  Holds all the windows for the gsh app and functions to initialize and 
 *  update them.
*/

#include <ncurses.h>

extern WINDOW* infow;
extern WINDOW* barw;
extern WINDOW* menuw;
extern WINDOW* inputw;

void createWindows();

void wupdate(WINDOW* win, char* str);

int wverase(int nmWins, ...);

int wvrefresh(int nmWins, ...);

//#define wgetinput(win, str) wgetninput(win, str, DEFAULT_MAX_IN_SIZE)

//int wgetninput(WINDOW* win, char* str, int num);

void cleanupWindows();

#endif
