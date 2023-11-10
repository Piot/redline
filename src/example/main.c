/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <redline/edit.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>

typedef struct test_app {
    redline_edit edit;
} test_app;

static int g_quit = 0;

static void interruptHandler(int sig)
{
    g_quit = 1;
}

static void drawPrompt(test_app* self)
{
    redline_edit_prompt(&self->edit, "hello> ");
}

int main(int argc, char* argv[])
{
    test_app self;

    redline_edit_init(&self.edit);
    drawPrompt(&self);

    signal(SIGINT, interruptHandler);

    int time = 0;
    while (!g_quit) {
        int result = redline_edit_update(&self.edit);
        usleep(10000);
        int timeToPrint = (time++ % 60) == 0;
        if (timeToPrint) {
            redline_edit_remove(&self.edit);
            printf("Output from remote computer\nIs it displayed here?\n");
            drawPrompt(&self);
            redlineEditBringback(&self.edit);
        }
        if (result == -1) {
            const char* textInput = redline_edit_line(&self.edit);
            if (strcmp(textInput, "quit") == 0) {
                break;
            }

            redline_edit_clear(&self.edit);
            printf("\n");
            drawPrompt(&self);
            redline_edit_reset(&self.edit);
        }
    }

    redline_edit_close(&self.edit);

    return 0;
}