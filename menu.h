#ifndef MENU_H
#define MENU_H

#define MAX_LABEL_SIZE 80

typedef struct MenuOpt MenuOpt;
typedef struct Menu Menu;

struct Menu {
    Menu* parent;
    int nmOpts;
    MenuOpt** opts;
};

struct MenuOpt {
    char label[MAX_LABEL_SIZE];
    Menu* child; 
}; 

extern Menu* mainMenu;

MenuOpt* createMenuOpt(char* label, Menu* child);

// puts a new menu in menu
Menu* createMenu(Menu* menu, int nmOpts, char* labels[MAX_LABEL_SIZE]);

// prints menu with the hlChoice-th choice highlighted
void wprintmenu(WINDOW* win, Menu* menu, int hlChoice);

void delmenu(Menu* menu);

#endif
