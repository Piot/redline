/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/redline
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <breathe/breathe_app.h>
#include <clog/clog.h>

#include <redline/edit.h>

typedef struct test_app {
    redline_edit edit;
} test_app;

static test_app app;

static void prompt(test_app* self)
{
    redline_edit_prompt(&self->edit, "hello> ");
}

int g_breathe_draw(void* _self)
{
    test_app* self = (test_app*) _self;
    int result = redline_edit_update(&self->edit);
    if (result == -1) {
        redline_edit_clear(&self->edit);
        printf("\n");
        prompt(self);
        redline_edit_reset(&self->edit);
    }

    return 0;
}

void* g_breathe_init(int argc, const char* argv[], int width, int height)
{
    test_app* self = (test_app*) &app;
    redline_edit_init(&self->edit);
    prompt(self);
    return self;
}

void g_breathe_close(void* _self)
{
    test_app* self = (test_app*) _self;
    redline_edit_close(&self->edit);
    printf("\nresult: '%s'\n", self->edit.text_input.s);
}
