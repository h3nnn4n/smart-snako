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

#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "agent_utils.h"
#include "raw_hamilton_agent.h"

bool build_halmiton_with_dfs(grid_t *grid, uint8_t x, uint8_t y) {
    raw_hamilton_context_t *agent_context = (raw_hamilton_context_t *)grid->agent_context;
    static direction_t      directions[4] = {RIGHT, DOWN, UP, LEFT};

    if (x == 0 && y == 0 && agent_context->path[x][y].visited) {
        printf("yay\n");
        return true;
    }

    if (agent_context->path[x][y].visited)
        return false;

    agent_context->path[x][y].visited = true;

    for (int i = 0; i < 4; i++) {
        direction_t direction                    = directions[i];
        agent_context->path[x][y].next_direction = direction;

        uint8_t new_x = x;
        uint8_t new_y = y;

        switch (direction) {
            case RIGHT: new_x++; break;
            case LEFT: new_x--; break;
            case UP: new_y--; break;
            case DOWN: new_y++; break;
        }

        if (new_x >= grid->width || new_y >= grid->height)
            continue;

        // True means we found the target node (0, 0)
        if (build_halmiton_with_dfs(grid, new_x, new_y))
            return true;
    }

    return false;
}

void raw_hamilton_agent_create(grid_t *grid) {
    grid->agent_context = malloc(sizeof(raw_hamilton_context_t));
    memset(grid->agent_context, 0, sizeof(raw_hamilton_context_t));

    raw_hamilton_context_t *agent_context = (raw_hamilton_context_t *)grid->agent_context;

    tuple_t *path = malloc(sizeof(tuple_t) * grid->width * grid->height);
    memset(path, 0, sizeof(tuple_t) * grid->width * grid->height);

    agent_context->path = malloc(sizeof(tuple_t *) * grid->width);
    memset(agent_context->path, 0, sizeof(tuple_t *) * grid->width);

    for (uint8_t i = 0; i < grid->width; i++) {
        agent_context->path[i] = &path[i * grid->height];
    }

    bool result = build_halmiton_with_dfs(grid, 0, 0);
    if (!result)
        printf("WARN: failed to build halmitonian cycle\n");
}

void raw_hamilton_agent_destroy(grid_t *grid) {
    assert(grid->agent_context != NULL);

    raw_hamilton_context_t *agent_context = (raw_hamilton_context_t *)grid->agent_context;
    free(agent_context->path[0]);
    free(agent_context->path);

    free(grid->agent_context);
}

direction_t raw_hamilton_agent(grid_t *grid) {
    raw_hamilton_context_t *context = (raw_hamilton_context_t *)grid->agent_context;

    uint8_t x = grid->snake_head_x;
    uint8_t y = grid->snake_head_y;

    direction_t next_direction = context->path[x][y].next_direction;

    if (is_snake_colliding(grid, next_direction))
        return get_safe_random_direction(grid);

    return next_direction;
}
