/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <redline/text_input.h>
#include <stdio.h>
#include <string.h>
#include <tiny-libc/tiny_libc.h>
#include <fcntl.h>

static int read_key(RedlineTextInput* self)
{
#if defined TORNADO_OS_WINDOWS
    DWORD charactersRead;

    HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);

    BOOL hasInputEvents = GetNumberOfConsoleInputEvents(stdinHandle, &charactersRead);
    if (!hasInputEvents) {
        return EOF;
    }

    if (charactersRead > 0) {
        DWORD eventsReadCount;
        INPUT_RECORD record[2];
        ReadConsoleInputA(stdinHandle, record, 1, &eventsReadCount);
        if (eventsReadCount > 0) {
            if (record[0].EventType == KEY_EVENT) {
                if (record[0].Event.KeyEvent.bKeyDown) {
                    return record[0].Event.KeyEvent.uChar.AsciiChar;
                }
            }
        }
        return EOF;
    }
    return EOF;
#else
    (void) self;
    return fgetc(self->stdIn);
#endif
}

static void insert(RedlineTextInput* self, int c_char)
{
    for (size_t i = self->input_length + 1; i > self->cursor_x; --i) {
        self->s[i] = self->s[i - 1];
    }
    self->s[self->cursor_x] = (char) c_char;
    self->input_length++;
    self->touched++;
}

static void delete(RedlineTextInput* self)
{
    for (size_t i = self->cursor_x; i < self->input_length; ++i) {
        self->s[i] = self->s[i + 1];
    }
    --self->input_length;
    self->touched++;
}

static void delete_pressed(RedlineTextInput* self)
{
    if (self->cursor_x < self->input_length) {
        delete (self);
    }
}

static void set_from_history(RedlineTextInput* self)
{
    const char* h = redlineHistoryCurrent(&self->history);
    if (!h) {
        self->s[0] = 0;

    } else {
        strcpy(self->s, h);
    }
    size_t len = strlen(self->s);
    self->input_length = len;
    self->cursor_x = len;
    self->touched++;
}

static void cursor_up(RedlineTextInput* self)
{
    redlineHistoryPrevious(&self->history);
    set_from_history(self);
}

static void cursor_down(RedlineTextInput* self)
{
    redlineHistoryNext(&self->history);
    set_from_history(self);
}

static void cursor_start_of_line(RedlineTextInput* self)
{
    self->cursor_x = 0;
}

static void cursor_end_of_line(RedlineTextInput* self)
{
    self->cursor_x = self->input_length;
}

static int default_char(RedlineTextInput* self, int c_char)
{
    switch (self->escape_phase) {
        case 0: {
            if (c_char >= 32 && c_char < 127) {
                insert(self, c_char);
                self->cursor_x++;
            }
        } break;
        case 1: {
            if (c_char == '[') {
                self->escape_phase = 2;
            } else if (c_char == 'O') {
                self->escape_phase = 3;
            }
        } break;
        case 2: {
            switch (c_char) {
                case 'D': {
                    if (self->cursor_x > 0) {
                        self->cursor_x--;
                    }
                } break;
                case 'C': {
                    if (self->cursor_x < self->input_length) {
                        self->cursor_x++;
                    }
                } break;
                case 0x48: {
                    cursor_start_of_line(self);
                } break;
                case 0x41: {
                    cursor_up(self);
                } break;
                case 0x42: {
                    cursor_down(self);
                } break;
                case 0x46: {
                    cursor_end_of_line(self);
                } break;
                case 0x31:
                case 0x32: {
                    self->escape_phase = 4;
                    return 0;
                }
                case 0x33: {
                    delete_pressed(self);
                } break;
                default: {
                    printf("\n\n%02X\n\n", c_char);
                }
            }
            self->escape_phase = 0;
        } break;
        case 3: {
            if (c_char >= 'P' && c_char <= 'S') {
                int fn = c_char - 'P';
                printf("Func%d\n", fn + 1);
            }
            self->escape_phase = 0;
        } break;
        case 4: {
            self->escape_phase = 0;
            switch (c_char) {
                case 0x35:
                    return REDLINE_KEY_F5;
                case 0x37:
                    return REDLINE_KEY_F6;
                case 0x38:
                    return REDLINE_KEY_F7;
                case 0x39:
                    return REDLINE_KEY_F8;
                case 0x30:
                    return REDLINE_KEY_F9;
            }
            printf("wF%d\n", c_char);

        } break;
    }

    return 0;
}

int redlineTextInputUpdate(RedlineTextInput* self)
{
    if (self->input_length >= self->max_length) {
        return 0;
    }
    int c_char = read_key(self);
    if (c_char != EOF) {
        // printf("%02X\n", c_char);
    }
    switch (c_char) {
        case EOF:
            return 0;
        case 1: // Start of line
            cursor_start_of_line(self);
            break;
        case 5: // End of line
            cursor_end_of_line(self);
            break;
        case 27: {
            self->escape_phase = 1;
        } break;
        case 126: { // DEL
                    // delete_pressed(self);
        } break;
        case 127:
        case 8: // Windows support
        {       // BACKSPACE
            if (self->cursor_x > 0) {
                self->cursor_x--;
                delete (self);
                c_char = 0;
            } else {
                c_char = 0;
            }
        } break;
        case 10:
        case 13: // Windows support
        {
            redlineHistoryAdd(&self->history, self->s);
            return -1;
        }
        default:
            return default_char(self, c_char);
    }

    return 1;
}

void redlineTextInputInit(RedlineTextInput* self, int stdIn)
{
    self->input_length = 0;
    self->max_length = 255;
    self->touched = 0;
    self->escape_phase = 0;
    self->stdInFileDescriptor = stdIn;

    self->stdIn = fdopen(self->stdInFileDescriptor, "r");
    redlineTextInputClear(self);
    redlineHistoryInit(&self->history);
}

void redlineTextInputClear(RedlineTextInput* self)
{
    self->input_length = 0;
    self->s[0] = 0;
    self->s[1] = 0;
    self->cursor_x = 0;
}

const char* redlineTextInputLine(RedlineTextInput* self)
{
    return self->s;
}
