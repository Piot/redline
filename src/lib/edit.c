/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <redline/ansi.h>
#include <redline/edit.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined TORNADO_OS_WINDOWS
#include <unistd.h>
#endif

#if !defined TORNADO_OS_WINDOWS
static void enable_raw_mode(RedlineEdit* self)
{
    self->stdInFileDescriptor = STDIN_FILENO;
    tcgetattr(self->stdInFileDescriptor, &self->oldt);
    self->newt = self->oldt;
    // self->newt.c_iflag &= (tcflag_t) ~(INPCK | ISTRIP | IXON | IXOFF); // BRKINT
    // self->newt.c_lflag &= (tcflag_t) ~(ECHONL | ICANON | ECHO | );
    self->newt.c_lflag &= (tcflag_t) ~(ECHONL | ICANON | ECHO | IEXTEN);
    // newt.c_oflag &= ~(OPOST);
    // self->newt.c_cc[VMIN] = 1; // needed when disabling icanon
    // self->newt.c_cc[VTIME] = 0; // needed when disabling icanon

    tcsetattr(self->stdInFileDescriptor, TCSANOW, &self->newt);
}
#endif

static void restoreTermMode(RedlineEdit* self)
{
    redlineAnsiColorReset();
#if !defined TORNADO_OS_WINDOWS
    tcsetattr(self->stdInFileDescriptor, TCSANOW, &self->oldt);
#endif
}

void redlineEditInit(RedlineEdit* self)
{
#if !defined TORNADO_OS_WINDOWS
    enable_raw_mode(self);
#else
#endif
    redlineAnsiInit();
    redlineTextInputInit(&self->text_input, self->stdInFileDescriptor);
    redlineSyncInit(&self->sync);
}

int redlineEditUpdate(RedlineEdit* self)
{
    int result = 1;

    while (result == 1) {
        result = redlineTextInputUpdate(&self->text_input);
    }
    redlineSyncPrint(&self->sync, self->text_input.s, self->text_input.cursor_x, self->text_input.touched);
    return result;
}

void redlineEditRemove(RedlineEdit* self)
{
    (void) self;
    redlineAnsiCursorFirstColumn();
    redlineAnsiColorReset();
    redlineAnsiClearEndOfLine();
}

void redlineEditBringback(RedlineEdit* self)
{
    self->text_input.touched++;
    size_t x = self->text_input.cursor_x;
    self->text_input.cursor_x = 0;
    redlineEditUpdate(self);
    self->text_input.cursor_x = x;
}

void redlineEditClear(RedlineEdit* self)
{
    redlineTextInputClear(&self->text_input);
}

void redlineEditReset(RedlineEdit* self)
{
    redlineEditClear(self);
    redlineSyncPrintReset(&self->sync);
}

void redlineEditPrompt(RedlineEdit* self, const char* prompt)
{
    (void) self;
    redlineAnsiColor(3);
    printf("%s", prompt);
    redlineAnsiColorReset();
}

const char* redlineEditLine(RedlineEdit* self)
{
    return redlineTextInputLine(&self->text_input);
}

void redlineEditAddHistoryStrings(RedlineEdit* self, const char* history)
{
    redlineHistoryAddStrings(&self->text_input.history, history);
}

void redlineEditClose(RedlineEdit* self)
{
    restoreTermMode(self);
}
