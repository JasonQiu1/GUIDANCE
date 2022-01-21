#define MAX_FILE_LINE_LEN 512

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mainmenu.xxd"
#include "menu.h"
#include "strutil.h"
#include "window.h"

// Creates an entire menu and submenus from a file
// format is as follow:
// MenuName NumSubMenus 
// Menu tooltip
// numEvents event1 event2 event3 ...
// SubMenu1 NumberOfMenuOptions
// Submenu tooltip
// numEvents event1 event2 event3 ...
// ...

// requires "%ms" - "m" assignment-allocation character from POSIX
static char menuLine[MAX_FILE_LINE_LEN];
static Menu* loadMenuFromFileAux(FILE* fpMenu, Menu* super) {
    int sumNmChConsumed;
    int nmChConsumed;
    Menu* menu = malloc(sizeof *menu);
    menu->super = super;

    // Get label and number of submenus
    if (fgets(menuLine, MAX_FILE_LINE_LEN, fpMenu) == NULL) {
        perror("get label line");
    }
    if (sscanf(menuLine, "%ms %d", &menu->label, &menu->nmSubs) != 2) {
        perror("extract label and numSubmenu from label line"); 
    }
    menu->subs = malloc(sizeof **menu->subs * menu->nmSubs);

    // Get tooltip
    if (fgets(menuLine, MAX_FILE_LINE_LEN, fpMenu) == NULL) {
        perror("get tooltip line");
    }
    sscanf(strskipspace(menuLine), "%m[^\n]", &menu->tooltip);

    // Get number of events and events
    sumNmChConsumed = 0;
    if (fgets(menuLine, MAX_FILE_LINE_LEN, fpMenu) == NULL) {
        perror("get events line");
    }
    if (sscanf(menuLine, "%d%n", &menu->nmEvents, &nmChConsumed) != 1) {
        perror("extract numEvents from events line");
    }
    menu->events = malloc(sizeof *menu->events * menu->nmEvents);
    for (int i = 0; i < menu->nmEvents; i++) {
        sumNmChConsumed += nmChConsumed;
        if (sscanf(menuLine + sumNmChConsumed, "%ms%n", &menu->events[i], &nmChConsumed) != 1) {
            perror("extract an event from the events line");
        }
    }

    // Get sub menus
    for (int i = 0; i < menu->nmSubs; i++) {
        menu->subs[i] = loadMenuFromFileAux(fpMenu, menu);
    }
    
    return menu;
}

//static Menu* loadMenuFromFile(char* fileName) {
//    FILE* fpMenu = fopen(fileName, "r");
//    if (fpMenu == NULL) {
//        fprintf(stderr, "Could not open '%s'.\n", fileName);
//        exit(-1);
//    }
//    Menu* menu = loadMenuFromFileAux(fpMenu, NULL);
//    fclose(fpMenu);
//    return menu;
//}

// load main menu from variable "char* main_menu" with "int main_menu_len"
// which is included from "mainmenu.xxd"
Menu* initMenu() {
    // requires fmemopen from POSIX
    FILE* fpmain_menu = fmemopen(main_menu, main_menu_len, "r");
    Menu* mainMenu = loadMenuFromFileAux(fpmain_menu, NULL);
    fclose(fpmain_menu);

    return mainMenu;
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
