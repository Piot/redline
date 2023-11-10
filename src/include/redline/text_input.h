/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef REDLINE_TEXT_INPUT_H
#define REDLINE_TEXT_INPUT_H

#include <stddef.h>

#include "history.h"

typedef struct RedlineTextInput {
    char s[256];
    size_t input_length;
    size_t max_length;
    size_t cursor_x;
    int escape_phase;
    size_t touched;
    RedlineHistory history;
} RedlineTextInput;

void redlineTextInputInit(RedlineTextInput* self);
int redlineTextInputUpdate(RedlineTextInput* self);
void redlineTextInputClear(RedlineTextInput* self);
void redlineTextInputReset(RedlineTextInput* self);
const char* redlineTextInputLine(RedlineTextInput* self);

#define REDLINE_KEY_F1 (1)
#define REDLINE_KEY_F2 (2)
#define REDLINE_KEY_F3 (3)
#define REDLINE_KEY_F4 (4)
#define REDLINE_KEY_F5 (5)
#define REDLINE_KEY_F6 (6)
#define REDLINE_KEY_F7 (7)
#define REDLINE_KEY_F8 (8)
#define REDLINE_KEY_F9 (9)
#define REDLINE_KEY_F10 (10)
#define REDLINE_KEY_F11 (11)
#define REDLINE_KEY_F12 (12)

#endif
