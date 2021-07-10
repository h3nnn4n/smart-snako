/*
 * Copyright <2021> <Renan S Silva, aka h3nnn4n>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "raw_hamilton_agent.h"

void raw_hamilton_agent_create(grid_t *grid) {
    grid->agent_context = malloc(sizeof(direction_t));
    memset(grid->agent_context, 0, sizeof(raw_hamilton_context_t));
}

void raw_hamilton_agent_destroy(grid_t *grid) {
    assert(grid->agent_context != NULL);
    free(grid->agent_context);
}

direction_t raw_hamilton_agent(grid_t *grid) {
    raw_hamilton_context_t *context = (raw_hamilton_context_t *)grid->agent_context;

    context->stuff = !context->stuff;

    if (context->stuff)
        return RIGHT;

    return DOWN;
}
