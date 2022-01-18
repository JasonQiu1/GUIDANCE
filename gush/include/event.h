#ifndef EVENT_H
#define EVENT_H

/*
 *  Handles events. The events array is setup as a map from event name to
 *  function pointer, and is initialized in eventmap.c in the build dir
 *
*/

// Initializes everything event needs.
void initEvent();

// Runs the function corresponding to eventName.
// Returns -1 if the event/function cannot be found,
//         1 if a validation event finds that the passed data is invalid
//         0 if everything is good
int handleEvent(char* eventName);

void eventBootup();

char* eventPressOther(char inputCh);

#endif
