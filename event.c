/*
 *  All event functions live here. The map containing the 
 *  function pointers and strings are auto-generated at eventmap.h.
 *
 *  event functions take one argument:
 *      if there is text on the userInput line, then that is passed in,
 *      if empty, then the label of the pressed menu is passed instead
 *
 *  there are 5 main types of events:
 *  pressKEY - do stuff when KEY is pressed.
 *  infoX - update the info window to the "binder" named X
 *  barHl - change the infobar text to the passed value and highlight it
 *  validateX - validate the userInput against X
 *  dataX- update X in the user's data file using the input
*/

#define MAX_IN_LEN 80
#define INPUT_PROMPT_STR "$ "

#include <ncurses.h>
#include <string.h>

#include "binder.h"
#include "event.h"
#include "log.h" // extern logInfo[MAX_LOG_INFO_LEN]
#include "menu.h"
#include "window.h" // extern WINDOW* infow, barw, menuw, inputw

static char userInput[MAX_IN_LEN];
static int nmInputted = 0;

static int eventRet = -1;
static Menu* currMenu = NULL;
static int selSub = 0;
static Binder* binder = NULL;

static int pressUp() {
    for (int i = 0; i < currMenu->subs[selSub]->nmEvents; i++) {
        eventRet = handleEvent(currMenu->subs[selSub]->events[i]);
        if (eventRet == -1) {
            snprintf(logInfo, MAX_LOG_INFO_LEN, "'%s' event not found!", 
                     currMenu->subs[selSub]->events[i]);
            logPrint(CRITICAL, logInfo);
        } else if (eventRet == 1) {
            // TODO: update highlight bar to display a try again prompt
            //handleEvent("barHl", "Invalid userInput, try again!");
        }
    }

    if (currMenu->subs[selSub]->nmSubs) {
        currMenu = currMenu->subs[selSub];
        selSub = 0;
        wupdate(barw, currMenu->subs[selSub]->tooltip);

        werase(menuw);
        wprintmenu(menuw, currMenu, selSub);
        wrefresh(menuw);
    }
    return 0;
}

static int pressDown() {
    if (currMenu->super) {
        currMenu = currMenu->super;
        selSub = 0;
        wupdate(barw, currMenu->subs[selSub]->tooltip);

        werase(menuw);
        wprintmenu(menuw, currMenu, selSub);
        wrefresh(menuw);

        // when event handling when going to a previous menu, don't pass any userInput and don't handle returns 
        for (int i = 0; i < currMenu->nmEvents; i++) {
            eventRet = handleEvent(currMenu->events[i]);
        }
    }
    return 0;
}

static int pressLeft() {
    selSub--;
    if (selSub < 0) selSub = 0;
    wupdate(barw, currMenu->subs[selSub]->tooltip);
    wprintmenu(menuw, currMenu, selSub);
    wrefresh(menuw);
    return 0;
}

static int pressRight() {
    selSub++;
    if (selSub >= currMenu->nmSubs) selSub = currMenu->nmSubs-1;
    wupdate(barw, currMenu->subs[selSub]->tooltip);
    wprintmenu(menuw, currMenu, selSub);
    wrefresh(menuw);
    return 0;
}

static int pressEnter() {
    userInput[nmInputted++] = 0;
    wupdate(barw, userInput);
    wupdate(inputw, INPUT_PROMPT_STR);
    memset(userInput, 0, 4);
    nmInputted = 0;
    return 0;
}

// Assume userInput is null terminated.
// Delete, backspace, etc.
static int row = -1, col = -1;
static int pressDelete() {
    if (nmInputted > 0) {
        nmInputted--;
        getyx(inputw, row, col);
        wmove(inputw, row, col - 1);
        wdelch(inputw);
        wrefresh(inputw);
    }
    return 0;
}

static int infoMain() {
    werase(infow);
    mvwprintw(infow, 0, 0, "Info window: This is the info window.\nPress the left and right arrow keys to move around the menu.\nPress the up arrow key to press the selected menu option, and hit the down key to go back to the previous menu.\nType in stuff and hit enter for it to pop up in the info box.\n\nType in exit to leave.");
    wrefresh(infow);
    return 0;
}

static int infoSecondBinder() {
    werase(infow);
    mvwprintw(infow, 0, 0, "This is the first page of a second info binder.");
    wrefresh(infow);
    return 0;
}

// This is the map containing all of event names and function pointers.
const static struct {
    const char* name;
    int (*funcp)();
} events[] = {
    #include "eventmap.h" // all of the keys
};

// Runs the function corresponding to eventName.
// Passes the name of the selected menu label if no input.
// Returns -1 if the event/function cannot be found,
//         1 if a validation event finds that the passed data is invalid
//         0 if everything is good
int handleEvent(char* eventName) {
    for (int i = 0; i < sizeof(events) / sizeof(events[0]); i++) {
        if (!strcmp(eventName, events[i].name)) {
            return (*events[i].funcp)();
        }
    }
    return -1;
}

void initEvent() {
    currMenu = initMenu();
}

void eventBootup() {
    createWindows();

    selSub = 0;
    box(stdscr, 0, 0); 
    wprintmenu(menuw, currMenu, selSub);
    wvrefresh(2, stdscr, menuw);

    infoMain("");
    wupdate(barw, currMenu->subs[selSub]->tooltip);
    wupdate(inputw, INPUT_PROMPT_STR);
}

char* eventPressOther(char inputCh) {
    if (inputCh >= 32 && inputCh <= 255 && nmInputted < MAX_IN_LEN - 1) {
        userInput[nmInputted++] = inputCh;
        waddch(inputw, inputCh);
        wrefresh(inputw);
    } else {
        printf("\a");
    }
    return userInput;
}

