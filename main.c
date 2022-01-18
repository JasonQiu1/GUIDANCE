/*  
 *  This is where the main function lives. 
 *  It runs all of the init functions in the programs,
 *  runs the input loop, and cleans up memory after the user exits.
*/

#include <ncurses.h> // extern WINDOW* stdscr
#include <stdio.h>
#include <string.h>

#include "event.h" // MAX_IN_LEN
#include "log.h" // MAX_LOG_INFO_LEN
#include "ncursesutil.h"
#include "window.h" // extern WINDOW* inputw

void inputLoop() {
    int inputCh;
    int loop = 1;

    eventBootup();

    while (loop) {
        inputCh = wgetch(inputw);
        switch(inputCh) {
            case KEY_UP:
                handleEvent("pressUp");
                break;
            case KEY_DOWN:
                handleEvent("pressDown");
                break;
            case KEY_LEFT:
                handleEvent("pressLeft");
                break;
            case KEY_RIGHT:
                handleEvent("pressRight");
                break;
            case '[':
                handleEvent("pressOpenBracket");
                break;
            case ']':
                handleEvent("pressCloseBracket");
                break;
            case 127:
            case KEY_DL:
            case KEY_BACKSPACE:
                handleEvent("pressDelete");
                break;
            case '\n':
            case KEY_ENTER:
                handleEvent("pressEnter");
                break;
            default:
                if (!strncmp(eventPressOther(inputCh), "exit", 4)) {
                    loop = 0;
                }
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (initLog() != 0) return -1;
    initEvent();
    initNcurses();

    logPrint(PROGINFO, "Started gsh.");
    snprintf(logInfo, MAX_LOG_INFO_LEN, "Terminal size: %dx%d", lines, columns);
    logPrint(PROGINFO, logInfo);

    inputLoop();

    cleanupWindows();
    cleanupNcurses();

    return 0;
}
