/*
 * This is the interface to the menu system. 
 * It initializes the mainMenu, 
 * provides functions to print the menu to an ncurses window,
 * and a function to cleanup the menu.
*/

#ifndef MENU_H
#define MENU_H

#define MAX_TOOLTIP_SIZE 160
#define MAX_LABEL_SIZE 80
#define MAX_EVENT_NAME_SIZE 80
#define MAX_MENU_FILE_LINE_SIZE 512

// Menus have a super menu and optionally submenus.
// When menus are entered, they trigger all of their events.
// Label is the display name of the menu when being shown as a submenu.
// Tooltip is the info displayed on the infobar when the label is hovered over.
typedef struct Menu Menu;
struct Menu {
    Menu* super;
    int nmSubs;
    Menu** subs;
    int nmEvents;
    char (*events)[MAX_EVENT_NAME_SIZE];
    char label[MAX_LABEL_SIZE];
    char tooltip[MAX_TOOLTIP_SIZE];
};

extern Menu* mainMenu;

// Loads the main menu
void initMenu();

// Prints a menu to a window with the hlChoice-th choice highlighted.
void wprintmenu(WINDOW* win, Menu* menu, int hlChoice);

// Deallocates a menu and all submenus.
void delmenu(Menu* menu);

#endif
