#include <ncurses.h>
#include <stdarg.h>
#include <term.h>

#include "window.h"

WINDOW* infow = NULL;
WINDOW* infobarw = NULL;
WINDOW* interactw = NULL;

void createWindows() {
    // Info window starts on top and takes up most of screen.
    // Info bar takes a bit of the middle
    // Interactive window takes up few lines at the bottom.
    infow = subwin(stdscr, 
            lines - INFOBARW_LINES - INTERACTW_LINES - TOP_MARG - BOT_MARG, columns - LEFT_MARG - RIGHT_MARG, TOP_MARG, LEFT_MARG);
    if (!infow) {
        fprintf(stderr, "infow init failed\n"); 
    }
    infobarw = subwin(stdscr, 
            INFOBARW_LINES, columns - LEFT_MARG - RIGHT_MARG, 
            lines - INFOBARW_LINES - INTERACTW_LINES - BOT_MARG, LEFT_MARG);
    if (!infobarw) {
        fprintf(stderr, "infobarw init failed\n");
    }
    interactw = subwin(stdscr,
                       INTERACTW_LINES, columns - LEFT_MARG - RIGHT_MARG,
                       lines - INTERACTW_LINES - BOT_MARG, LEFT_MARG);
    if (!interactw) {
        fprintf(stderr, "interactw init failed\n");
    }
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
