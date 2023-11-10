/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef REDLINE_ANSI_H
#define REDLINE_ANSI_H

#include <stdint.h>
#include <stddef.h>

void redlineAnsiInit(void);
void redlineAnsiCursorBack(size_t n);
void redlineAnsiCursorFirstColumn(void);
void redlineAnsiClearEndOfLine(void);

void redlineAnsiColor(uint8_t index);
void redlineAnsiColorReset(void);

#endif
