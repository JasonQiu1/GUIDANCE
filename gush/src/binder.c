#define BINDER_CREATE_PAGE_LIMIT 5

#include <stdlib.h>
#include <string.h>

#include "binder.h"

// returns the number of characters starting at str that take up a full page
// returns -1 if makes up less than a full page (i.e. no characters left)
static int currRow;
static int currCol;
static int nmChs;
static int strseppage(char* str, int pageMaxX, int pageMaxY) {
    currRow = currCol = nmChs = 0; 
    for (char* pStr = str; *pStr; pStr++, nmChs++) {
        if (*pStr == '\n' || currCol == pageMaxY) {
            currRow++;
            currCol = 0;
        } else {
            currCol++;
        }

        if (currRow == pageMaxX) {
            return nmChs; 
        }
    }
    return -1;
}

// Separates content into multiple pages if content is large enough 
Binder* createBinder(char* content, int pageMaxX, int pageMaxY) {
    Binder* tempBinder;
    int pageSize, offset = 0, nextPageNmChs = strseppage(content, pageMaxX, pageMaxY);
    Binder* newBinder = malloc(sizeof *newBinder);
    newBinder->prev = NULL;
    newBinder->next = NULL;

    if (nextPageNmChs == -1) {
        pageSize = strlen(content) + 1;
        newBinder->content = malloc(sizeof *newBinder->content * pageSize);
        strncpy(newBinder->content, content, pageSize);
        newBinder->content[pageSize] = '\0';
        return newBinder;
    }

    pageSize = nextPageNmChs + 1;
    newBinder->content = malloc(sizeof *newBinder->content * pageSize);
    strncpy(newBinder->content, content, pageSize);
    newBinder->content[pageSize] = '\0';

    offset += pageSize;
    tempBinder = newBinder;
    while (1) {
        Binder* newPage = malloc(sizeof *newPage);
        newPage->prev = NULL;
        newPage->next = NULL;
        tempBinder->next = newPage;
        newPage->prev = tempBinder;

        nextPageNmChs = strseppage(content + offset, pageMaxX, pageMaxY);
        if (nextPageNmChs == -1) {
            pageSize = strlen(content) + 1;
            newPage->content = malloc(sizeof *newPage->content * pageSize);
            strncpy(newPage->content, content + offset, pageSize);
            break;
        }
        newPage->content = malloc(sizeof *newPage->content * nextPageNmChs + 1);
        strncpy(newPage->content, content + offset, nextPageNmChs + 1);
        tempBinder->content[nextPageNmChs] = '\0';
        offset += nextPageNmChs + 1;

        tempBinder = newPage;
    }

    return newBinder;
}

// Frees a binder.
void delBinder(Binder* binder) {
    if (!binder) return;

    // flip to second-left-most page
    while (binder->prev && binder->prev->prev) {
        binder = binder->prev;
    }
    if (!binder->prev && binder->next) binder = binder->next;

    // free previous page and flip right until nothing left
    while (binder->prev && binder->next) {
        free(binder->prev->content);
        free(binder->prev);
        binder = binder->next;
    }
    if (binder->prev) {
        free(binder->prev->content);
        free(binder->prev);
    }
    free(binder->content);
    free(binder);
}
