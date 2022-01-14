#ifndef WINDOW_H
#define WINDOW_H
#include <ncurses.h>

#define TOP_MARG 1
#define LEFT_MARG 1
#define BOT_MARG 1
#define RIGHT_MARG 1

#define BARW_LINES 3
#define MENUW_LINES 1
#define INPUTW_LINES 1

extern WINDOW* infow;
extern WINDOW* barw;
extern WINDOW* menuw;
extern WINDOW* inputw;

void createWindows();

int wverase(int nmWins, ...);

int wvrefresh(int nmWins, ...);

#define wgetinput(win, str) wgetninput(win, str, DEFAULT_MAX_IN_SIZE)

int wgetninput(WINDOW* win, char* str, int num);

void cleanupWindows();

#endif
