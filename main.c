#include <ncurses.h>
#include <term.h>
#include <stdio.h>
#include <string.h>

#include "ncursesutil.h"
#include "window.h"

#define DEFAULT_MAX_IN_SIZE 80

void doStuff() {
    char userInput[80] = {0};

    box(stdscr, 0, 0); 
    wrefresh(stdscr);

    while (strcmp(userInput, "exit")) {
        wverase(3, infow, infobarw, interactw);

        mvwprintw(infow, 0, 0, "Info window: This is the info window");
        mvwprintw(infobarw, 0, 0,
                "Infobar: %s", userInput);
        mvwprintw(interactw, 0, 0,
                "Interactive window: ");

        wvrefresh(2, infow, infobarw);

        wgetinput(interactw, userInput);
    }
}

int main(int argc, char* argv[]) {
    // Set up windows
    initNcurses();
    createWindows();

    fprintf(stderr, "Terminal size: %dx%d\n", lines, columns);

    doStuff();

    // Cleanup windows 
    cleanup();
}
