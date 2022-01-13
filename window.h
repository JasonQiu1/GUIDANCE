#ifndef WINDOW_H
#define WINDOW_H
#include <ncurses.h>

#define TOP_MARG 1
#define LEFT_MARG 1
#define BOT_MARG 1
#define RIGHT_MARG 1

#define INFOBARW_LINES 2
#define INTERACTW_LINES 4

extern WINDOW* infow;
extern WINDOW* infobarw;
extern WINDOW* interactw;

void createWindows();

int wverase(int nmWins, ...);

int wvrefresh(int nmWins, ...);

#define wgetinput(win, str) wgetninput(win, str, DEFAULT_MAX_IN_SIZE)

int wgetninput(WINDOW* win, char* str, int num);

#endif
