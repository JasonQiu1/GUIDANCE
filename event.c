/*
 *  All event functions live here. The map containing the 
 *  function pointers and strings are auto-generated at eventmap.h.
 *
 *  event functions take one argument:
 *      if there is text on the input line, then that is passed in,
 *      if empty, then the label of the pressed menu is passed instead
 *
 *  there are 4 main types of events:
 *  infoX - update the info window to the "binder" named X
 *  barHl - change the infobar text to the passed value and highlight it
 *  validateX - validate the input against X
 *  dataX- update X in the user's data file using the input
*/

#include <ncurses.h>
#include <string.h>

#include "event.h"
#include "log.h"
#include "menu.h"
#include "window.h"

static int infoMain(char* input) {
    wrefresh(infow);
    werase(infow);
    wrefresh(infow);
    mvwprintw(infow, 0, 0, "Info window: This is the info window.\nPress the left and right arrow keys to move around the menu.\nPress the up arrow key to press the selected menu option, and hit the down key to go back to the previous menu.\nType in stuff and hit enter for it to pop up in the info box.\n\nType in exit to leave.");
    wrefresh(infow);
    return 0;
}

static int infoSecondBinder(char* input) {
    werase(infow);
    mvwprintw(infow, 0, 0, "This is the first page of a second info binder.");
    wrefresh(infow);
    return 0;
}

// This is the map containing all of event names and function pointers.
const static struct {
    const char* name;
    int (*funcp)(char*);
} events[] = {
    #include "eventmap.h" // all of the keys
};

// Runs the function corresponding to eventName.
// Returns -1 if the event/function cannot be found,
//         1 if a validation event finds that the passed data is invalid
//         0 if everything is good
int handleEvent(char* eventName, char* input) {
    for (int i = 0; i < sizeof(events) / sizeof(events[0]); i++) {
        if (!strcmp(eventName, events[i].name)) {
            return (*events[i].funcp)(input);
        }
    }
    return -1;
}

