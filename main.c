#include <ncurses.h>
#include <term.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "ncursesutil.h"
#include "window.h"

#define MAX_IN_SIZE 80

Menu* mainMenu = NULL;

void updateBar(char* str) {
    werase(barw);
    mvwprintw(barw, 0, 0, "Infobar: %s", str);
    wrefresh(barw);
}

void doStuff() {
    char userInput[MAX_IN_SIZE] = {0};
    int nmChInputted = 0;
    int inputCh, row, col;
    Menu* currMenu = mainMenu;
    int selSub = 0;

    box(stdscr, 0, 0); 
    mvwprintw(infow, 0, 0, "Info window: This is the info window.\nPress the left and right arrow keys to move around the menu.\nPress the up arrow key to press the selected menu option, and hit the down key to go back to the previous menu.\nType in stuff and hit enter for it to pop up in the info box.\n\nType in exit to leave.");
    updateBar(currMenu->subs[selSub]->tooltip);
    wprintmenu(menuw, currMenu, selSub);
    mvwprintw(inputw, 0, 0, "Input window: ");
    wvrefresh(5, stdscr, infow, barw, menuw, inputw);

    while (strncmp(userInput, "exit", 4)) {
        inputCh = wgetch(inputw);
        switch(inputCh) {
            case KEY_DOWN:
                if (currMenu->super) {
                    currMenu = currMenu->super;
                    selSub = 0;
                    updateBar(currMenu->subs[selSub]->tooltip);

                    werase(menuw);
                    wprintmenu(menuw, currMenu, selSub);
                    wrefresh(menuw);
                }
                break;
            case KEY_UP:
                if (currMenu->subs[selSub]->nmSubs) {
                    currMenu = currMenu->subs[selSub];
                    selSub = 0;
                    updateBar(currMenu->subs[selSub]->tooltip);

                    werase(menuw);
                    wprintmenu(menuw, currMenu, selSub);
                    wrefresh(menuw);
                } else {
                    // ACTIVATE EVENTS -- TODO
                    updateBar(currMenu->subs[selSub]->events[0]);
                }
                break;
            case KEY_LEFT:
                selSub--;
                if (selSub < 0) selSub = 0;
                updateBar(currMenu->subs[selSub]->tooltip);
                wprintmenu(menuw, currMenu, selSub);
                wrefresh(menuw);
                break;
            case KEY_RIGHT:
                selSub++;
                if (selSub >= currMenu->nmSubs) selSub = currMenu->nmSubs-1;
                updateBar(currMenu->subs[selSub]->tooltip);
                wprintmenu(menuw, currMenu, selSub);
                wrefresh(menuw);
                break;
            case 127:
                if (nmChInputted > 0) {
                    nmChInputted--;
                    getyx(inputw, row, col);
                    wmove(inputw, row, col - 1);
                    wdelch(inputw);
                    wrefresh(inputw);
                }
                break;
            case '\n':
                userInput[nmChInputted++] = 0;
                nmChInputted = 0;

                updateBar(userInput);

                werase(inputw);
                mvwprintw(inputw, 0, 0, "Input window: ");
                wrefresh(inputw);
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

    mainMenu = loadMenuFromFile("./main.menu");

    fprintf(stderr, "Terminal size: %dx%d\n", lines, columns);

    doStuff();

    cleanupWindows();
    cleanupNcurses();

    return 0;
}
