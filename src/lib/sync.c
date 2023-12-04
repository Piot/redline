/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <redline/ansi.h>
#include <redline/sync.h>
#include <stdio.h>
#include <string.h>

void redlineSyncPrint(RedlineSync* self, const char* s, size_t x, size_t touched)
{
    if (self->touched == touched && x == self->cursor_x) {
        return;
    }
    self->touched = touched;
    redlineAnsiCursorBack(self->cursor_x);
    redlineAnsiColor(10);
    size_t len = strlen(s);
    printf("%s", s);
    size_t pos_x = len;
    if (len < self->length) {
        size_t space_count = self->length - len;
        for (size_t i = 0; i < space_count; ++i) {
            printf(" ");
        }
        pos_x = self->length;
    }
    self->pos_x = pos_x;
    self->cursor_x = x;
    self->length = len;
    redlineAnsiCursorBack(self->pos_x - self->cursor_x);
    redlineAnsiColorReset();
    fflush(stdout);
}

void redlineSyncInit(RedlineSync* self)
{
    self->cursor_x = 0;
    self->length = 0;
    self->pos_x = 0;
}

void redlineSyncPrintReset(RedlineSync* self)
{
    self->pos_x = 0;
    self->cursor_x = 0;
    self->length = 0;
    self->touched = 0;
}
