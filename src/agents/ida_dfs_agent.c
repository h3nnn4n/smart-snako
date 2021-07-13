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

#include "agent_utils.h"
#include "ida_dfs_agent.h"

void ida_dfs_agent_create(grid_t *grid) {
    grid->agent_context = malloc(sizeof(ida_dfs_agent_context_t));
    memset(grid->agent_context, 0, sizeof(ida_dfs_agent_context_t));

    ida_dfs_agent_context_t *agent_context = (ida_dfs_agent_context_t *)grid->agent_context;

    graph_context_t *graph_context = create_graph_context(grid);
    agent_context->graph_context   = graph_context;
}

void ida_dfs_agent_destroy(grid_t *grid) {
    assert(grid->agent_context != NULL);

    ida_dfs_agent_context_t *agent_context = (ida_dfs_agent_context_t *)grid->agent_context;

    destroy_graph_context(agent_context->graph_context);

    free(grid->agent_context);
}

direction_t ida_dfs_agent(grid_t *grid) {
    ida_dfs_agent_context_t *context       = (ida_dfs_agent_context_t *)grid->agent_context;
    graph_context_t *        graph_context = context->graph_context;

    uint8_t x = grid->snake_head_x;
    uint8_t y = grid->snake_head_y;

    context->path_found = ida_dfs(graph_context, x, y);

    if (context->path_found)
        return graph_context->path[x][y].next_direction;

    return get_safe_random_direction(grid);
}
