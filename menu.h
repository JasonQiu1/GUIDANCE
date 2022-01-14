#ifndef MENU_H
#define MENU_H

#define MAX_LABEL_SIZE 80

typedef struct MenuOpt MenuOpt;

typedef struct Menu {
    int nmOpts;
    MenuOpt** opts;
} Menu;

struct MenuOpt {
    char label[MAX_LABEL_SIZE];
    Menu* parent;
    Menu* child; 
}; 

extern Menu* mainMenu;

MenuOpt* createMenuOpt(char* label, Menu* parent, Menu* child);

// puts a new menu in menu
Menu* createMenu(int nmOpts, char* labels[MAX_LABEL_SIZE]);

// prints menu with the hlChoice-th choice highlighted
void wprintmenu(WINDOW* win, Menu* menu, int hlChoice);

void delmenu(Menu* menu);

#endif
