/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef REDLINE_EDIT_H
#define REDLINE_EDIT_H

#include "sync.h"
#include "text_input.h"
#if !defined TORNADO_OS_WINDOWS
#include <termios.h>
#endif

typedef struct RedlineEdit {
    RedlineTextInput text_input;
    RedlineSync sync;
#if !defined TORNADO_OS_WINDOWS
    struct termios oldt, newt;
#endif
} RedlineEdit;

void redlineEditInit(RedlineEdit* self);
int redlineEditUpdate(RedlineEdit* self);
void redlineEditClose(RedlineEdit* self);
void redlineEditClear(RedlineEdit* self);
void redlineEditRemove(RedlineEdit* self);
void redlineEditReset(RedlineEdit* self);
void redlineEditPrompt(RedlineEdit* self, const char* prompt);
const char* redlineEditLine(RedlineEdit* self);
void redlineEditAddHistoryStrings(RedlineEdit* self, const char* history);
void redlineEditBringback(RedlineEdit* self);

#endif
