#include <locale.h>
#include <curses.h>
#include <term.h>
#include <stdio.h>

#define TOP_MARG 1
#define LEFT_MARG 1
#define BOT_MARG 1
#define RIGHT_MARG 1

#define INFOBARW_LINES 2
#define INTERACTW_LINES 4

#define TOP_PAD 1
#define LEFT_PAD 2

WINDOW* infow = NULL;
WINDOW* infobarw = NULL;
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
    char userInput[80] = {0};

    box(stdscr, 0, 0); 
    wrefresh(stdscr);
    mvwprintw(infow, 0, 0, "Info window: This is the info window");
    wrefresh(infow);
    mvwprintw(infobarw, 0, 0,
              "Infobar: Useful info!");
    wrefresh(infobarw);
    mvwprintw(interactw, 0, 0,
              "Interactive window: ");
    wrefresh(interactw);

    wgetnstr(interactw, userInput, 80);
}

void cleanup() {
    delwin(infow);
    delwin(infobarw);
    delwin(interactw);
    endwin();
}

int main(int argc, char* argv[]) {
    // Set up windows
    initNcurses();

    // Info window starts on top and takes up most of screen.
    // Interactive window takes up few lines at the bottom.
    infow = subwin(stdscr, 
            lines - INFOBARW_LINES - INTERACTW_LINES - TOP_MARG - BOT_MARG, columns - LEFT_MARG - RIGHT_MARG, TOP_MARG, LEFT_MARG);
    if (!infow) {
        fprintf(stderr, "infow init\n"); 
    }
    infobarw = subwin(stdscr, 
            INFOBARW_LINES, columns - LEFT_MARG - RIGHT_MARG, 
            lines - INFOBARW_LINES - INTERACTW_LINES - BOT_MARG, LEFT_MARG);
    if (!infobarw) {
        fprintf(stderr, "infobarw init\n");
    }
    interactw = subwin(stdscr,
                       INTERACTW_LINES, columns - LEFT_MARG - RIGHT_MARG,
                       lines - INTERACTW_LINES - BOT_MARG, LEFT_MARG);
    if (!interactw) {
        fprintf(stderr, "interactw init\n");
    }

    fprintf(stderr, "Terminal size: %dx%d", lines, columns);

    if (!ferror(stderr)) {
        doStuff();
    }

    // Cleanup windows 
    cleanup();
}
