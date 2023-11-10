/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef REDLINE_SYNC_H
#define REDLINE_SYNC_H

#include <stddef.h>

typedef struct RedlineSync {
    size_t cursor_x;
    size_t pos_x;
    size_t length;
    size_t touched;
} RedlineSync;

void redlineSyncInit(RedlineSync* self);
void redlineSyncPrint(RedlineSync* self, const char* s, size_t x, size_t touched);
void redlineSyncPrintReset(RedlineSync* self);

#endif
