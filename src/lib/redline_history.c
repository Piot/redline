/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <redline/history.h>
#include <tiny-libc/tiny_libc.h>

void redlineHistoryPrevious(RedlineHistory* self)
{
    if (self->index != 0) {
        self->index--;
    }
}

void redlineHistoryNext(RedlineHistory* self)
{
    if (self->index < self->count) {
        self->index++;
    }
}

const char* redlineHistoryCurrent(RedlineHistory* self)
{
    if (self->count == 0) {
        return "";
    }
    return self->strings[self->index];
}

/*
static int existing(RedlineHistory* self, const char* s)
{
    for (size_t i = 0; i < self->count; ++i) {
        if (tc_str_equal(self->strings[i], s)) {
            return (int)i;
        }
    }

    return -1;
}
*/

static char* duplicate_string(const char* source)
{
    size_t source_size;
    char* target;
    char* target_ptr;

    source_size = strlen(source);
    target = (char*) malloc(sizeof(char) * (size_t)source_size + 1);
    if (target == 0) {
        return 0;
    }

    target_ptr = target;
    while (*source) {
        *target_ptr++ = *source++;
    }
    *target_ptr = '\0';

    return target;
}

void redlineHistoryAdd(RedlineHistory* self, const char* s)
{
    if (tc_strlen(s) == 0) {
        return;
    }
    if (self->count && tc_str_equal(self->strings[self->count - 1], s)) {
    } else {
        self->strings[self->count++] = duplicate_string(s);
    }
    self->index = self->count;
}

void redlineHistoryAddStrings(RedlineHistory* self, const char* history)
{
    char temp[128];
    tc_strncpy(temp, 128, history, 128);

    char* p = temp;
    while (1) {
        char* found_string = strtok(p, "\n");
        if (!found_string) {
            break;
        }
        redlineHistoryAdd(self, found_string);
        p += strlen(found_string) + 1;
    }
}

void redlineHistoryInit(RedlineHistory* self)
{
    self->max_count = 0;
    self->index = 0;
    self->count = 0;
}
