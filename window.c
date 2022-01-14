#include <ncurses.h>
#include <stdarg.h>
#include <term.h>

#include "window.h"

WINDOW* infow = NULL;
WINDOW* barw = NULL;
WINDOW* menuw = NULL;
WINDOW* inputw = NULL;

void createWindows() {
    // Info window starts on top and takes up most of screen.
    // Info bar takes a bit of the middle
    // Menu window takes a line in between
    // Input window takes up few lines at the bottom.
    infow = subwin(stdscr, 
            lines - TOP_MARG - BOT_MARG - INPUTW_LINES - MENUW_LINES - MENUW_LINES, columns - LEFT_MARG - RIGHT_MARG, TOP_MARG, LEFT_MARG);
    if (!infow) {
        fprintf(stderr, "infow init failed\n"); 
    }
    barw = subwin(stdscr, 
            BARW_LINES, columns - LEFT_MARG - RIGHT_MARG, 
            lines - BOT_MARG - INPUTW_LINES - MENUW_LINES - BARW_LINES, LEFT_MARG);
    if (!barw) {
        fprintf(stderr, "barw init failed\n");
    }
    menuw = subwin(stdscr,
            MENUW_LINES, columns - LEFT_MARG - RIGHT_MARG,
            lines - BOT_MARG - INPUTW_LINES - MENUW_LINES, LEFT_MARG);
    if (!menuw) {
        fprintf(stderr, "menuw init failed\n");
    }
    inputw = subwin(stdscr, 
            INPUTW_LINES, columns - LEFT_MARG - RIGHT_MARG, lines - INPUTW_LINES - BOT_MARG, LEFT_MARG);
    if (!inputw) {
        fprintf(stderr, "inputw init failed\n");
    }
    intrflush(inputw, FALSE);
    keypad(inputw, TRUE);
}
int wverase(int nmWins, ...) {
    int ret = 0;
    va_list wins;
    va_start(wins, nmWins);

    for (int i = 0; i < nmWins; i++) {
        ret |= werase(va_arg(wins, WINDOW*));
    }

    va_end(wins);
    return ret;
}

int wvrefresh(int nmWins, ...) {
    int ret = 0;
    va_list wins;
    va_start(wins, nmWins);

    for (int i = 0; i < nmWins; i++) {
        ret |= wrefresh(va_arg(wins, WINDOW*));
    }

    va_end(wins);
    return ret;
}

int wgetninput(WINDOW* win, char* str, int num) {
    int ret = 0;

    nocbreak(); echo();
    ret = wgetnstr(win, str, num);
    cbreak(); noecho();

    return ret;
}

void cleanupWindows() {
    delwin(infow);
    delwin(barw);
    delwin(menuw);
    delwin(inputw);
}
