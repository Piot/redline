/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef REDLINE_HISTORY_H
#define REDLINE_HISTORY_H

#include <stddef.h>

typedef struct RedlineHistory {
    const char* strings[100];
    size_t index;
    size_t count;
    size_t max_count;
} RedlineHistory;

void redlineHistoryInit(RedlineHistory* self);
void redlineHistoryPrevious(RedlineHistory* self);
void redlineHistoryNext(RedlineHistory* self);
const char* redlineHistoryCurrent(RedlineHistory* self);
void redlineHistoryAdd(RedlineHistory* self, const char* s);
void redlineHistoryAddStrings(RedlineHistory* self, const char* history);

#endif
