#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "window.h"

#define STR(x) _STR(x)
#define _STR(x) #x

char* trimLeadingWhitespace(char* str) {
    char* pStr = str;
    while (*pStr == ' ') pStr++;
    return pStr;
}

// Creates an entire menu and submenus from a file
// format is as follow:
// MenuName NumSubMenus 
// Menu tooltip
// numEvents event1 event2 event3 ...
// SubMenu1 NumberOfMenuOptions
// Submenu tooltip
// numEvents event1 event2 event3 ...
// ...
static char menuLine[MAX_MENU_FILE_LINE_SIZE];
static Menu* loadMenuFromFileAux(FILE* fpMenu, Menu* super) {
    int sumNmChConsumed;
    int nmChConsumed;
    Menu* menu = malloc(sizeof *menu);
    menu->super = super;

    // Get label and number of submenus
    if (fgets(menuLine, MAX_MENU_FILE_LINE_SIZE, fpMenu) == NULL) {
        perror("get label line");
    }
    if (sscanf(menuLine, "%" STR(MAX_LABEL_SIZE) "s %d", menu->label, &menu->nmSubs) != 2) {
        perror("extract label and numSubmenu from label line"); 
    }
    menu->subs = malloc(sizeof **menu->subs * menu->nmSubs);

    // Get tooltip
    if (fgets(menuLine, MAX_MENU_FILE_LINE_SIZE, fpMenu) == NULL) {
        perror("get tooltip line");
    }
    strncpy(menu->tooltip, trimLeadingWhitespace(menuLine), MAX_TOOLTIP_SIZE);

    // Get number of events and events
    sumNmChConsumed = 0;
    if (fgets(menuLine, MAX_MENU_FILE_LINE_SIZE, fpMenu) == NULL) {
        perror("get events line");
    }
    if (sscanf(menuLine, "%d%n", &menu->nmEvents, &nmChConsumed) != 1) {
        perror("extract numEvents from events line");
    }
    menu->events = malloc(sizeof *menu->events * menu->nmEvents);
    for (int i = 0; i < menu->nmEvents; i++) {
        sumNmChConsumed += nmChConsumed;
        if (sscanf(menuLine + sumNmChConsumed, "%" STR(MAX_EVENT_NAME_SIZE) "s%n", menu->events[i], &nmChConsumed) != 1) {
            perror("extract an event from the events line");
        }
    }

    // Get sub menus
    for (int i = 0; i < menu->nmSubs; i++) {
        menu->subs[i] = loadMenuFromFileAux(fpMenu, menu);
    }
    
    return menu;
}

#undef STR
#undef _STR

Menu* loadMenuFromFile(char* fileName) {
    FILE* fpMenu = fopen(fileName, "r");
    if (fpMenu == NULL) {
        fprintf(stderr, "Could not open '%s'.\n", fileName);
        exit(-1);
    }
    Menu* menu = loadMenuFromFileAux(fpMenu, NULL);
    fclose(fpMenu);
    return menu;
}

void wprintmenu(WINDOW* win, Menu* menu, int hlChoice) {
    wmove(win, 0, 0);
    for (int i = 0; i < menu->nmSubs; i++) {
        if (i == hlChoice) {
            wattron(win, A_REVERSE);
            wprintw(win, menu->subs[i]->label);
            wattroff(win, A_REVERSE);
        } else {
            wprintw(win, menu->subs[i]->label);
        }
        if (i != menu->nmSubs - 1) {
            waddch(win, ' ');
        }
    }
}

// recursively deallocates bottom-up
void delmenu(Menu* menu) {
    //TODO: make it
}
