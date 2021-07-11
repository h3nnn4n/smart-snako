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
#include "raw_hamilton_agent.h"

void print_path(grid_t *grid) {
    raw_hamilton_context_t *agent_context = (raw_hamilton_context_t *)grid->agent_context;
    graph_context_t *       graph_context = agent_context->graph_context;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (graph_context->path[x][y].visited) {
                switch (graph_context->path[x][y].next_direction) {
                    case RIGHT: printf("> "); break;
                    case LEFT: printf("< "); break;
                    case UP: printf("^ "); break;
                    case DOWN: printf("V "); break;
                }
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

bool build_halmiton_with_dfs(grid_t *grid, uint8_t x, uint8_t y) {
    raw_hamilton_context_t *agent_context   = (raw_hamilton_context_t *)grid->agent_context;
    graph_context_t *       graph_context   = agent_context->graph_context;
    static direction_t      directions[4]   = {RIGHT, DOWN, UP, LEFT};
    static direction_t      directions_2[4] = {LEFT, RIGHT, UP, DOWN};

    // The two left-most columns, whe the width is odd and we are not on the first row
    bool enable_odd_width_hack = (grid->width % 2 == 1) && (x < 2) && (y > 0);

    // When both the rows and columns are an odd number
    bool enable_odd_height_hack = (grid->height % 2 == 1) && (grid->width % 2 == 1);

    if (x == 0 && y == 0 && graph_context->path[x][y].visited) {
        if (get_config()->verbose) {
            printf("\n");
            print_path(grid);
            printf("\n");
        }

        if (all_cells_visited(graph_context)) {
            return true;
        }
        /*printf("aaaa %u %u\n", cells_not_visited_count(grid), enable_odd_height_hack);*/

        if (cells_not_visited_count(graph_context) == 1 && enable_odd_height_hack) {
            graph_context->path[1][2].hack = true;

            /*printf("hack\n");*/
            return true;
        }
    }

    if (graph_context->path[x][y].visited)
        return false;

    graph_context->path[x][y].visited = true;

    for (int i = 0; i < 4; i++) {
        direction_t direction = directions[i];

        if (enable_odd_width_hack)
            direction = directions_2[i];

        graph_context->path[x][y].next_direction = direction;

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

    graph_context->path[x][y].visited = false;

    return false;
}

void raw_hamilton_agent_create(grid_t *grid) {
    grid->agent_context = malloc(sizeof(raw_hamilton_context_t));
    memset(grid->agent_context, 0, sizeof(raw_hamilton_context_t));

    raw_hamilton_context_t *agent_context = (raw_hamilton_context_t *)grid->agent_context;

    graph_context_t *graph_context = create_graph_context(grid);
    agent_context->graph_context   = graph_context;

    bool result = build_halmiton_with_dfs(grid, 0, 0);
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
