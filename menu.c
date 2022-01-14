#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "window.h"

Menu* mainMenu;

MenuOpt* createMenuOpt(char* label, Menu* parent, Menu* child) {
    MenuOpt* newOpt = (MenuOpt*)malloc(sizeof(MenuOpt));
    strncpy(newOpt->label, label, MAX_LABEL_SIZE);
    newOpt->parent = parent;
    newOpt->child = child;
    return newOpt;
}

Menu* createMenu(int nmOpts, char* labels[MAX_LABEL_SIZE]) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    menu->nmOpts = nmOpts;
    menu->opts = (MenuOpt**)malloc(sizeof(MenuOpt*) * nmOpts);
    for (int i = 0; i < nmOpts; i++) {
        menu->opts[i] = createMenuOpt(labels[i], menu, NULL);
    }
    return menu;
}

void wprintmenu(WINDOW* win, Menu* menu, int hlChoice) {
    wmove(win, 0, 0);
    for (int i = 0; i < menu->nmOpts; i++) {
        if (i == hlChoice) {
            wattron(win, A_REVERSE);
            wprintw(win, menu->opts[i]->label);
            wattroff(win, A_REVERSE);
        } else {
            wprintw(win, menu->opts[i]->label);
        }
        if (i != menu->nmOpts - 1) {
            waddch(win, ' ');
        }
    }
}

// recursively deallocates bottom-up
void delmenu(Menu* menu) {
    // TODO: actually recurse down and dealloc everything
    for (int i = 0; i < menu->nmOpts; i++) {
        free(menu->opts[i]);
    }
    free(menu->opts);
    free(menu);
}
