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
#include "dfs.h"
#include "dfs_agent.h"

void dfs_agent_create(grid_t *grid) {
    grid->agent_context = malloc(sizeof(dfs_agent_context_t));
    memset(grid->agent_context, 0, sizeof(dfs_agent_context_t));

    dfs_agent_context_t *agent_context = (dfs_agent_context_t *)grid->agent_context;

    graph_context_t *graph_context = create_graph_context(grid);
    agent_context->graph_context   = graph_context;
}

void dfs_agent_destroy(grid_t *grid) {
    assert(grid->agent_context != NULL);

    dfs_agent_context_t *agent_context = (dfs_agent_context_t *)grid->agent_context;

    destroy_graph_context(agent_context->graph_context);

    free(grid->agent_context);
}

direction_t dfs_agent(grid_t *grid) {
    dfs_agent_context_t *context       = (dfs_agent_context_t *)grid->agent_context;
    graph_context_t *    graph_context = context->graph_context;

    uint8_t x = grid->snake_head_x;
    uint8_t y = grid->snake_head_y;

    uint8_t cherry_x, cherry_y;
    get_cherry_position(grid, &cherry_x, &cherry_y);
    /*context->path_found = dfs(graph_context, x, y);*/

    if (context->path_found && context->cherry_x == cherry_x && context->cherry_y == cherry_y) {
        /*if (context->path_found) {*/
        /*printf("following dfs\n");*/
        return graph_context->path[x][y].next_direction;
    }

    /*printf("looking for a new path\n");*/

    context->path_found = false;
    context->cherry_x   = cherry_x;
    context->cherry_y   = cherry_y;

    /*printf("starting dfs\n");*/
    context->path_found = dfs(graph_context, x, y);
    /*printf("finished dfs\n");*/
    direction_t next_direction = graph_context->path[x][y].next_direction;

    if (!context->path_found || is_snake_colliding(grid, next_direction)) {
        /*printf("path not found, or suicidal\n");*/
        /*return RIGHT;*/
        return get_safe_random_direction(grid);
    }

    /*printf("path found\n");*/
    return next_direction;
}
