#include <locale.h>

#include "ncursesutil.h"
#include "window.h"

// Initialize ncurses
void initNcurses() {
    setlocale(LC_ALL, "");
    initscr(); cbreak(); noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    refresh();
}

// Clean up windows
void cleanup() {
    delwin(infow);
    delwin(infobarw);
    delwin(interactw);
    endwin();
}
