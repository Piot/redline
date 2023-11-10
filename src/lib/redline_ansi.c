/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <flood/out_stream.h>
#include <redline/ansi.h>
#include <stdio.h>
#include <tinge/tinge.h>

static void escape(const char* s)
{
    printf("%c[%s", 27, s);
}

void redlineAnsiCursorBack(size_t n)
{
    if (n == 0) {
        return;
    }
    char buf[8];
    sprintf(buf, "%zuD", n);
    escape(buf);
}

void redlineAnsiCursorFirstColumn(void)
{
    printf("\r");
}

void redlineAnsiClearEndOfLine(void)
{
    escape("0K");
}

void redlineAnsiInit(void)
{
    escape("7l");
}

void redlineAnsiColor(uint8_t index)
{
    uint8_t temp[64];
    FldOutStream tempStream;
    TingeState tempState;
    fldOutStreamInit(&tempStream, temp, 64);
    tingeStateInit(&tempState, &tempStream);
    tingeStateFgColorIndex(&tempState, index);
    printf("%s", tingeStateToString(&tempState));
}

void redlineAnsiColorReset(void)
{
    uint8_t temp[64];
    FldOutStream tempStream;
    TingeState tempState;
    fldOutStreamInit(&tempStream, temp, 64);
    tingeStateInit(&tempState, &tempStream);
    tingeStateReset(&tempState);
    printf("%s", tingeStateToString(&tempState));
}
