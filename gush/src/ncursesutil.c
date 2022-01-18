#include <locale.h>
#include <ncurses.h>

#include "ncursesutil.h"

int lines = 0, columns = 0;

// Initialize ncurses
void initNcurses() {
    setlocale(LC_ALL, "");
    initscr(); cbreak(); noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    refresh();

    getmaxyx(stdscr, lines, columns);
}

// Clean up windows
void cleanupNcurses() {
    endwin();
}
