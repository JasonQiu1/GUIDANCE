#include <locale.h>
#include <ncurses/curses.h>
#include <ncurses/term.h>
#include <readline/readline.h>
#include <stdio.h>

#define INTERACTW_LINES 5

WINDOW* infow = NULL;
WINDOW* interactw = NULL;

// Initialize ncurses
void initNcurses() {
    setlocale(LC_ALL, "");
    initscr(); cbreak(); noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    refresh();
}

void doStuff() {
    mvwprintw(infow, 0, 0, 
              "Info window: This is the info window");
    wrefresh(infow);
    mvwprintw(interactw, 0, 0,
              "Interactive window: ");
    wrefresh(interactw);
    readline(""); 
}

int main(int argc, char* argv[]) {
    // Set up windows
    initNcurses();

    fprintf(stderr, "Terminal size: %dx%d", lines, columns);

    // Info window starts on top and takes up most of screen.
    // Interactive window takes up few lines at the bottom.
    infow = subwin(stdscr, 
                   lines - INTERACTW_LINES, columns,
                   0, 0);
    interactw = subwin(stdscr,
                       INTERACTW_LINES, columns,
                       lines - INTERACTW_LINES, 0);

    doStuff();

    // Cleanup windows 
    delwin(infow);
    endwin();
}
