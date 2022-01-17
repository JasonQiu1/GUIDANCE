#define BINDER_CREATE_PAGE_LIMIT 5

#include <stdlib.h>
#include <string.h>

#include "binder.h"

// Separates content into multiple pages of pageChLimit-1 chars per page
Binder* createBinder(char* content, int pageChLimit) {
    int contentLen = strnlen(content, BINDER_CREATE_PAGE_LIMIT * pageChLimit) + 1;
    Binder* newBinder = malloc(sizeof *newBinder);
    newBinder->content = malloc(sizeof *newBinder->content * ((contentLen < pageChLimit) ? contentLen : pageChLimit));
    strncpy(newBinder->content, content, pageChLimit); 
    newBinder->content[pageChLimit-1] = '\0';
    newBinder->prev = NULL;
    newBinder->next = NULL;
    Binder* b = newBinder;
    int currPos;
    for (int pageNo = 0; pageNo < contentLen / (pageChLimit - 1); pageNo++) {
        currPos = (pageChLimit - 1) * (pageNo + 1);
        Binder* newPage = malloc(sizeof *newPage);
        newPage->content = malloc(sizeof *newPage->content * (((contentLen - currPos) < pageChLimit) ? contentLen - currPos : pageChLimit));
        strncpy(newPage->content, content+currPos, pageChLimit);
        newPage->prev = b;
        newPage->next = NULL;
        b->next = newPage;
        b = newPage;
    }
    return newBinder;
}
