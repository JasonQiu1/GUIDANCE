#include <ncurses.h>
#include <term.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "ncursesutil.h"
#include "window.h"

#define MAX_IN_SIZE 80

void doStuff() {
    char userInput[MAX_IN_SIZE] = {0};
    int nmChInputted = 0;
    int inputCh;
    int selectedOpt = 0;

    box(stdscr, 0, 0); 
    wrefresh(stdscr);

    mvwprintw(infow, 0, 0, "Info window: This is the info window");
    mvwprintw(barw, 0, 0, "Infobar: %s", userInput);
    wprintmenu(menuw, mainMenu, selectedOpt);
    mvwprintw(inputw, 0, 0, "Input window: ");
    wvrefresh(4, infow, barw, menuw, inputw);
    while (strncmp(userInput, "exit", 4)) {
        inputCh = wgetch(inputw);
        switch(inputCh) {
            case KEY_UP: case KEY_DOWN:
                break;
            case KEY_LEFT:
                selectedOpt--;
                if (selectedOpt < 0) selectedOpt = 0;
                wprintmenu(menuw, mainMenu, selectedOpt);
                wrefresh(menuw);
                break;
            case KEY_RIGHT:
                selectedOpt++;
                if (selectedOpt >= mainMenu->nmOpts) 
                    selectedOpt = mainMenu->nmOpts-1;
                wprintmenu(menuw, mainMenu, selectedOpt);
                wrefresh(menuw);
                break;
            case KEY_ENTER:
                userInput[nmChInputted++] = 0;

                wverase(2, barw, inputw);
                mvwprintw(barw, 0, 0, "Infobar: %s", userInput);
                mvwprintw(inputw, 0, 0, "Input window: ");
                wvrefresh(2, barw, inputw);

                nmChInputted = 0;
                break;
            default:
                if (nmChInputted < MAX_IN_SIZE - 1) {
                    userInput[nmChInputted++] = inputCh;
                    waddch(inputw, inputCh);
                    wrefresh(inputw);
                } else {
                    printf("\a");
                }
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    initNcurses();
    createWindows();
    char* strs[MAX_LABEL_SIZE] = {"one", "two", "thr", "four"};
    mainMenu = createMenu(4, strs);

    fprintf(stderr, "Terminal size: %dx%d\n", lines, columns);

    doStuff();

    delmenu(mainMenu);
    cleanupWindows();
    cleanupNcurses();

    return 0;
}
