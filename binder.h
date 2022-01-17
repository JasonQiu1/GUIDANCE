#ifndef BINDER_H
#define BINDER_H

/*
 *  Binder: a collection of "pages" (strings) with max size
*/

typedef struct Binder Binder;
struct Binder {
    char* content;
    Binder* prev;
    Binder* next;
};

// Separates content into multiple pages of pageChLimit-1 chars per page
Binder* createBinder(char* content, int pageChLimit);

#endif
