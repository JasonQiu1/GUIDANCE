/*  
 *  This is where the main function lives. 
 *  It runs all of the init functions in the programs,
 *  runs the input loop, and cleans up memory after the user exits.
*/

#include <ncurses.h> // extern WINDOW* stdscr
#include <stdio.h>
#include <string.h>

#include "event.h"
#include "log.h"
#include "menu.h" // extern Menu* mainmenu
#include "ncursesutil.h"
#include "window.h" // extern WINDOW* infow, barw, menuw, inputw

#define MAX_IN_SIZE 80

char info[1028];
int lines = 0, columns = 0;

void updateBar(char* str) {
    werase(barw);
    mvwprintw(barw, 0, 0, "Infobar: %s", str);
    wrefresh(barw);
}

void inputLoop() {
    char userInput[MAX_IN_SIZE] = {0};
    int nmChInputted = 0;
    int inputCh, row, col;
    Menu* currMenu = mainMenu;
    int selSub = 0;
    Menu* m = NULL;
    int eventRet = 0;

    box(stdscr, 0, 0); 
    mvwprintw(infow, 0, 0, "Info window: This is the info window.\nPress the left and right arrow keys to move around the menu.\nPress the up arrow key to press the selected menu option, and hit the down key to go back to the previous menu.\nType in stuff and hit enter for it to pop up in the info box.\n\nType in exit to leave.");
    updateBar(currMenu->subs[selSub]->tooltip);
    wprintmenu(menuw, currMenu, selSub);
    mvwprintw(inputw, 0, 0, "Input window: ");
    wvrefresh(4, stdscr, infow, menuw, inputw);

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

                    // when event handling when going to a previous menu, don't pass any input and don't handle returns 
                    m = currMenu;
                    for (int i = 0; i < m->nmEvents; i++) {
                        eventRet = handleEvent(m->events[i], "");
                    }
                }
                break;
            case KEY_UP:
                m = currMenu->subs[selSub];
                for (int i = 0; i < m->nmEvents; i++) {
                    eventRet = handleEvent(m->events[i], ((nmChInputted) ? userInput : m->label));
                    if (eventRet == -1) {
                        snprintf(info, 1028, "'%s' event not found!", m->events[i]);
                        logPrint(CRITICAL, info);
                    } else if (eventRet == 1) {
                        handleEvent("barHl", "Invalid input, try again!");
                    }
                }

                if (m->nmSubs) {
                    currMenu = m;
                    selSub = 0;
                    updateBar(currMenu->subs[selSub]->tooltip);

                    werase(menuw);
                    wprintmenu(menuw, currMenu, selSub);
                    wrefresh(menuw);
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
                if (inputCh >= 32 && inputCh <= 255 && nmChInputted < MAX_IN_SIZE - 1) {
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
    initLog();
    initMenu();
    initNcurses();
    getmaxyx(stdscr, lines, columns);
    createWindows();

    logPrint(PROGINFO, "Started gsh.");
    snprintf(info, 128, "Terminal size: %dx%d", lines, columns);
    logPrint(PROGINFO, info);

    inputLoop();

    cleanupWindows();
    cleanupNcurses();

    return 0;
}
