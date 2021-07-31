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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cherry.h>
#include <config.h>
#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "graph_utils.h"
#include "hamilton_utils.h"
#include "raw_hamilton_agent.h"

void raw_hamilton_agent_create(grid_t *grid) {
    grid->agent_context = malloc(sizeof(raw_hamilton_context_t));
    memset(grid->agent_context, 0, sizeof(raw_hamilton_context_t));

    raw_hamilton_context_t *agent_context = (raw_hamilton_context_t *)grid->agent_context;

    graph_context_t *graph_context = create_graph_context(grid);
    agent_context->graph_context   = graph_context;

    bool result = build_halmiton_with_dfs(graph_context);
    if (!result)
        printf("WARN: failed to build halmitonian cycle\n");
}

void raw_hamilton_agent_destroy(grid_t *grid) {
    assert(grid->agent_context != NULL);

    raw_hamilton_context_t *agent_context = (raw_hamilton_context_t *)grid->agent_context;

    destroy_graph_context(agent_context->graph_context);

    free(grid->agent_context);
}

direction_t raw_hamilton_agent(grid_t *grid) {
    raw_hamilton_context_t *context       = (raw_hamilton_context_t *)grid->agent_context;
    graph_context_t *       graph_context = context->graph_context;

    uint8_t x = grid->snake_head_x;
    uint8_t y = grid->snake_head_y;

    direction_t next_direction = graph_context->path[x][y].next_direction;

    if (is_snake_colliding(grid, next_direction))
        return get_safe_random_direction(grid);

    if (graph_context->path[x][y].hack) {
        uint8_t cherry_x, cherry_y;
        get_cherry_position(grid, &cherry_x, &cherry_y);

        if (x == cherry_y && y == cherry_y + 1)
            return UP;
    }

    return next_direction;
}
